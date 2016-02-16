
// Please read Alex.h for information about the liscence and authors

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Alex.h"

#include "SmingCore.h"

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
