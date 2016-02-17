
#include "Arduino.h"
#include "Alex.h"
#include "SmingCore.h"

rBootHttpUpdate* otaUpdater
= 0;

void OtaUpdate_CallBack(bool result) { Serial.println("In callback...");

  if(!result) return (void) Serial.println("Firmware update failed!");
  
  uint8 slot = rboot_get_current_rom() ? 0 : 1;
  
  // set to boot new rom and then reboot
  Serial.printf("Firmware updated, rebooting to rom %d... (" __TIMESTAMP__ ")\r\n", slot);
  rboot_set_current_rom(slot);
  System.restart();
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

static HttpClient maker;

void onMake(HttpClient& client, bool successful);

bool makeOTA(){

  maker.downloadString("http://" OTA_IP ":3000/make", onMake);
}

void IRAM_ATTR interruptHandler() { Serial.println("Let's update vioa a button!");

  detachInterrupt(UPDATE_PIN);
  makeOTA();
  // OtaUpdate();
}

void onMake(HttpClient& client, bool successful)
{
  if (successful) {
    Serial.println("make was Successful! updating..");
    OtaUpdate();
  }
  else {

    Serial.println("Failed to make");
    attachInterrupt(UPDATE_PIN, interruptHandler, CHANGE);
  }
}

void Switch() {
  uint8 before = rboot_get_current_rom(), after = !before ? 1 : 0;
  Serial.printf("Swapping from rom %d to rom %d.\r\n", before, after);
  rboot_set_current_rom(after);
  Serial.println("Restarting...\r\n");
  System.restart();
}

void ShowInfo() {
    Serial.printf("\r\nSDK: v%s\r\nFree Heap: %d\r\nCPU Frequency: %d MHz\r\nSystem Chip ID: %x\r\nSPI Flash ID: %x\r\nBoot ROM: %lu\r\n", system_get_sdk_version(), system_get_free_heap_size(),system_get_cpu_freq(), system_get_chip_id(), spi_flash_get_id(), rboot_get_current_rom());
    //Serial.printf("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
}

void serialCallBack(Stream& stream, char arrivedChar, unsigned short availableCharsCount) {

  // Serial.println("INside serial callback");

  if (arrivedChar != '\n') return;
  char str[availableCharsCount];
  for (int i = 0; i < availableCharsCount; i++) {
    str[i] = stream.read();
    if (str[i] == '\r' || str[i] == '\n') str[i] = '\0';
  }
  
  if (!strcmp(str, "connect")) {
    // connect to wifi
    WifiStation.config(WIFI_SSID, WIFI_PWD);
    WifiStation.enable(true);
  } else if (!strcmp(str, "ip"))
    Serial.printf("ip: %s mac: %s\r\n", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str());
  else if (!strcmp(str, "ota"))       OtaUpdate();
  else if (!strcmp(str, "switch"))    Switch();
  else if (!strcmp(str, "restart"))   System.restart();
  else if (!strcmp(str, "ls")) {
    Vector<String> files = fileList();
    Serial.printf("filecount %d\r\n", files.count());
    for (unsigned int i = 0; i < files.count(); i++) {
      Serial.println(files[i]);
    }
  } else if (!strcmp(str, "cat")) {
    Vector<String> files = fileList();
    if (files.count() > 0) {
      Serial.printf("dumping file %s:\r\n", files[0].c_str());
      Serial.println(fileGetContent(files[0]));
    } else Serial.println("Empty spiffs!");
  } else if (!strcmp(str, "info")) ShowInfo();
  else if (!strcmp(str, "help")) {
    Serial.println();
    Serial.println("available commands:");
    Serial.println("  help - display this message");
    Serial.println("  ip - show current ip address");
    Serial.println("  connect - connect to wifi");
    Serial.println("  restart - restart the esp8266");
    Serial.println("  switch - switch to the other rom and reboot");
    Serial.println("  ota - perform ota update, switch rom and reboot");
    Serial.println("  info - show esp8266 info");
#ifndef DISABLE_SPIFFS
    Serial.println("  ls - list files in spiffs");
    Serial.println("  cat - show first file in spiffs");
#endif
    Serial.println();
  } else Serial.println("unknown command");
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


void alex_init() {
  
  Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
  // delay(2000);
  Serial.systemDebugOutput(true); // Debug output to serial
  
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

}


/*
HttpFirmwareUpdate airUpdater;

void IRAM_ATTR interruptHandler() {
  detachInterrupt(UPDATE_PIN);
  Serial.println("Let's do this cloud update!");
  airUpdater.start(); // Start cloud update
}

// Will be called when WiFi station was connected to AP
void connectOk() {

  Serial.println("I'm CONNECTED");

  // Configure cloud update
  airUpdater.addItem(0x0000, "http://" OTA_IP "/" X0_BIN);//simple.anakod.ru/fw/eagle.flash.bin");
  airUpdater.addItem(0x9000, "http://" OTA_IP "/" X9_BIN);

  attachInterrupt(UPDATE_PIN, interruptHandler, CHANGE);
  Serial.println("\r\nPress GPIO0 to start cloud update from " OTA_IP "! (" __TIMESTAMP__ ")\r\n");
}

void alex_init() {

  spiffs_mount(); // Mount file system, in order to work with files

  Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
  Serial.systemDebugOutput(true); // Debug output to serial

  WifiStation.enable(true);
  WifiStation.config(WIFI_SSID, WIFI_PWD);
  WifiAccessPoint.enable(false);

  // Run our method when station was connected to AP
  WifiStation.waitConnection(connectOk);
}

/*
Alex::Alex(uint8_t pin,unsigned long interval_millis)
{
	interval(interval_millis);
	previous_millis = millis();
	state = digitalRead(pin);
    this->pin = pin;
}


void Alex::write(int new_state)
       {
       	this->state = new_state;
       	digitalWrite(pin,state);
       }


void Alex::interval(unsigned long interval_millis)
{
  this->interval_millis = interval_millis;
  this->reAlex_millis = 0;
}

void Alex::reAlex(unsigned long interval)
{
	 this->reAlex_millis = interval;
}



int Alex::update()
{
	if ( deAlex() ) {
        reAlex(0);
        return stateChanged = 1;
    }

     // We need to reAlex, so simulate a state change
     
	if ( reAlex_millis && (millis() - previous_millis >= reAlex_millis) ) {
        previous_millis = millis();
		 reAlex(0);
		 return stateChanged = 1;
	}

	return stateChanged = 0;
}


unsigned long Alex::duration()
{
  return millis() - previous_millis;
}


int Alex::read()
{
	return (int)state;
}


// Protected: deAlexs the pin
int Alex::deAlex() {
	
	uint8_t newState = digitalRead(pin);
	if (state != newState ) {
  		if (millis() - previous_millis >= interval_millis) {
  			previous_millis = millis();
  			state = newState;
  			return 1;
	}
  }
  
  return 0;
	
}

// The risingEdge method is true for one scan after the de-Alexd input goes from off-to-on.
bool  Alex::risingEdge() { return stateChanged && state; }
// The fallingEdge  method it true for one scan after the de-Alexd input goes from on-to-off. 
bool  Alex::fallingEdge() { return stateChanged && !state; }

*/
