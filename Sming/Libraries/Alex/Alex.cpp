
// #include "Arduino.h"
// #include "Runloop.h"
#include "Alex.h"

#define X0_BIN "0x00000.bin"
#define X9_BIN "0x09000.bin"

#ifndef SPIFF_SIZE
#define SPIFF_SIZE 524288  //512K
#endif

#ifndef RBOOT_SPIFFS_0
#define RBOOT_SPIFFS_0 0x100000
#define RBOOT_SPIFFS_1  0x300000
#endif

Led led1 = Led(LEDPIN_1), myLed2 = Led(LEDPIN_2);


#pragma mark - OTA

rBootHttpUpdate* otaUpdater
= 0;

void OtaUpdate_CallBack(bool result) { Serial.println("In callback...");

  if(!result) { led1.off(); return (void) Serial.println("Firmware update failed!"); }
  
  uint8 slot = rboot_get_current_rom() ? 0 : 1;
  // set to boot new rom and then reboot
  Serial.printf("Firmware updated, rebooting to rom %d... (" __TIMESTAMP__ ")\r\n", slot);
  rboot_set_current_rom(slot); System.restart();
}

void OtaUpdate() { Serial.println("Updating...");
  
  // need a clean object, otherwise if run before and failed will not run again
  if (otaUpdater) delete otaUpdater; otaUpdater
  = new rBootHttpUpdate();
  
  // select rom slot to flash
  rboot_config bootconf = rboot_get_config();
  uint8 slot = !bootconf.current_rom ? 1 : 0;

  // flash appropriate rom
  otaUpdater->addItem(bootconf.roms[slot], !slot ? ROM_0_URL : ROM_1_URL);
#ifndef DISABLE_SPIFFS
  // use user supplied values (defaults for 4mb flash in makefile)
  otaUpdater->addItem(!slot ? RBOOT_SPIFFS_0 : RBOOT_SPIFFS_1, SPIFFS_URL);
#endif
  // request switch and reboot on success
  otaUpdater->switchToRom(slot);
  // and/or set a callback (called on failure or success without switching requested)
  otaUpdater->setCallback(OtaUpdate_CallBack);
  otaUpdater->start();
}

#pragma mark - remote ota maker

static HttpClient maker;
void onMake(HttpClient& client, bool successful);

bool makeOTA(){ maker.downloadString("http://" OTA_IP ":3000/make", onMake); }

void IRAM_ATTR interruptHandler() { Serial.println("Let's update vioa a button!");

  detachInterrupt(UPDATE_PIN); makeOTA(); // OtaUpdate();
}

void onMake(HttpClient& client, bool successful) {

  if (successful) return (void)Serial.println("make was Successful! updating.."), OtaUpdate();
  
  Serial.println("Failed to make"); attachInterrupt(UPDATE_PIN, interruptHandler, CHANGE);
}

#pragma mark - commands

void Switch() {
  uint8 before = rboot_get_current_rom(), after = !before ? 1 : 0;
  Serial.printf("Swapping from rom %d to rom %d.\r\n", before, after);
  rboot_set_current_rom(after);
  Serial.println("Restarting...\r\n");
  System.restart();
}

void ShowInfo() {
  
  Serial.printf("\r\nSDK: v%s\r\nFree Heap: %d\r\nCPU Frequency: %d MHz\r\nSystem Chip ID: %x\r\nSPI Flash ID: %x\r\nBoot ROM: %lu\r\nCompiled at: " __TIMESTAMP__ "\r\n", system_get_sdk_version(), system_get_free_heap_size(),system_get_cpu_freq(), system_get_chip_id(), spi_flash_get_id(), rboot_get_current_rom());
    //Serial.printf("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
}




void getSpiffy(int slot){
  
    if (!slot) {
#ifdef RBOOT_SPIFFS_0
    debugf("trying to mount spiffs at %x, length %d", RBOOT_SPIFFS_0 + 0x40200000, SPIFF_SIZE);
    spiffs_mount_manual(RBOOT_SPIFFS_0 + 0x40200000, SPIFF_SIZE);
#else
    debugf("trying to mount spiffs at %x, length %d", 0x40300000, SPIFF_SIZE);
    spiffs_mount_manual(0x40300000, SPIFF_SIZE);
#endif
  } else {
#ifdef RBOOT_SPIFFS_1
    debugf("trying to mount spiffs at %x, length %d", RBOOT_SPIFFS_1 + 0x40200000, SPIFF_SIZE);
    spiffs_mount_manual(RBOOT_SPIFFS_1 + 0x40200000, SPIFF_SIZE);
#else
    debugf("trying to mount spiffs at %x, length %d", 0x40500000, SPIFF_SIZE);
    spiffs_mount_manual(0x40500000, SPIFF_SIZE);
#endif
  }
}

/*
void IRAM_ATTR readJoystick(){

  Serial.println("reading D1");
  Serial.println(digitalRead(D1));

   Serial.println("reading A0");
  Serial.println(analogRead(D1));
  led1.onFor(300);

}

void IRAM_ATTR rising(){ Serial.println("Rise from your grave"); }
*/

// request switch and reboot on success// Will be called when WiFi station network scan was completed
void listNetworks(bool succeeded, BssList list) {

  if (!succeeded) return (void)Serial.println("Failed to scan networks");

  for (int i = 0; i < list.count(); i++)
    Serial.printf("\tWiFi: %s, %s%s\r\n", list[i].ssid, list[i].getAuthorizationMethodName(),
        list[i].hidden ? " (hidden)" : "");
}

void connect(){ WifiStation.config(WIFI_SSID, WIFI_PWD); WifiStation.enable(true); }

// Will be called when WiFi station was connected to AP
void connectOk()
{
  debugf("I'm CONNECTED");
  Serial.println(WifiStation.getIP().toString());
}

// Will be called when WiFi station timeout was reached
void connectFail()
{
  debugf("I'm NOT CONNECTED!");
  WifiStation.waitConnection(connectOk, 10, connectFail); // Repeat and check again
}

// Will be called when WiFi hardware and software initialization was finished
// And system initialization was completed
void ready()
{
  debugf("READY!");
  // If AP is enabled:
  debugf("AP. ip: %s mac: %s", WifiAccessPoint.getIP().toString().c_str(), WifiAccessPoint.getMAC().c_str());
}


const char * HELP_SCREEN = R"(
Available commands:
   
   help/? - display this message
       ip - show current ip address
  connect - connect to wifi
  restart - restart the esp8266
   switch - switch to the other rom and reboot
      ota - perform ota update, switch rom and reboot
   remake - remake libsming remotely using nodejs
     info - show esp8266 info
       ls - list files in spiffs
      cat - show first file in spiffs
)";


void serialCallBack(Stream& stream, char arrivedChar, unsigned short availableCharsCount) {

  if (arrivedChar != '\n') return;
  char str[availableCharsCount];
  for (int i = 0; i < availableCharsCount; i++) {
    str[i] = stream.read();
    if (str[i] == '\r' || str[i] == '\n') str[i] = '\0';
  }

  switchs(str) {

    icases("ip")
      Serial.printf("ip: %s mac: %s\r\n", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str());
      break;
    icases("connect")     connect();                    break;
    icases("remake")      makeOTA();                    break;
    icases("ota")         OtaUpdate();                  break;
    icases("switch")      Switch();                     break;
    icases("restart")     System.restart();             break;
    icases("info")        ShowInfo();                   break;
    icases("help")
    cases("?")            Serial.println(HELP_SCREEN);  break;
    icases("ls") 
      Vector<String> files = fileList();
      Serial.printf("filecount %d\r\n", files.count());
      for (unsigned int i = 0; i < files.count(); i++) Serial.println(files[i]);
      break;
    icases("cat")
      Vector<String> files = fileList();
      if (files.count() > 0) 
        Serial.printf("dumping file %s:\r\n%s\r\n", files[0].c_str(), fileGetContent(files[0]));
      else Serial.println("Empty spiffs!");
      break;
    defaults
      Serial.printf("No match\n");
      break;
    
    } switchs_end;
}

#include "Throttle.h"

Throttle throttle;

// uint8_t pins[ 1] = { D5 }; // List of pins that you want to connect to pwm  4erd
// HardwarePWM hwpwm(pins, 1);

void alex_init() {
  
  Serial.begin(SERIAL_BAUD_RATE); 
  Serial.systemDebugOutput(true); 


  // Set system ready callback method
  System.onReady(ready);

  // Print available access points
  WifiStation.startScan(listNetworks); // In Sming we can start network scan from init method without additional code

  // Run our method when station was connected to AP (or not connected)
  WifiStation.waitConnection(connectOk, 30, connectFail); // We recommend 20+ seconds at start

  /*
  led1.setTimer(1000);
  led1.blink(true);
  led2.setTimer(500);
  myLed2.blink(true);
  */

    // adcTimer.initializeMs(100, readJoystick()).start();
    
  // mount spiffs
  int slot = rboot_get_current_rom();

#ifndef DISABLE_SPIFFS
  getSpiffy(slot);
#else
  debugf("spiffs disabled");
#endif

  WifiAccessPoint.enable(false);
  
  Serial.printf("\r\nCurrently running rom %d.\r\nType 'help' and press enter for instructions.\r\n", slot);

  Serial.setCallback(serialCallBack);

  attachInterrupt(UPDATE_PIN, interruptHandler, CHANGE);
  Serial.printf("\r\nChange pin %i to start cloud update!\r\n", UPDATE_PIN);

  // attachInterrupt(D1, readJoystick, CHANGE);
  //attachInterrupt(D1, readJoystick, FALLING);
  //attachInterrupt(D1,  rising, RISING);

  // throttle._pwm = &hwpwm;
  // throttle.begin();

}



/*
void Stream_printf_progmem(Print &out, PGM_P format, ...) {
  
  // if (!serial_connected) return;
  // program memory version of printf - copy of format string and result share a buffer
  // so as to avoid too much memory use
  char formatString[128], *ptr;
  strncpy_P(formatString, format, sizeof(formatString)); // copy in from program mem
  // null terminate - leave last char since we might need it in worst case for result's \0
  formatString[sizeof(formatString)-2]='\0';
  ptr = &formatString[strlen(formatString)+1]; // our result buffer...
  va_list args;
  va_start (args,format);
  vsnprintf(ptr, sizeof(formatString)-1-strlen(formatString), formatString, args );
  va_end (args);
  formatString[sizeof(formatString)-1]='\0';
  out.print(ptr);
}
 
*/
