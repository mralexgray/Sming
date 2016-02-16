
/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 Main code by Thomas O Fredericks
 ReAlex and duration functions contributed by Eric Lowry
 Write function contributed by Jim Schimpf
 risingEdge and fallingEdge contributed by Tom Harkaway
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#define UPDATE_PIN 0 // GPIO0  aka D3

#define WIFI_SSID "Dickens"
#define WIFI_PWD "dickens1931"
 
#define OTA_IP "10.0.0.201"

 // download urls, set appropriately
#define ROM_0_URL  "http://" OTA_IP "/rom0.bin"
#define ROM_1_URL  "http://" OTA_IP "/rom1.bin"
#define SPIFFS_URL "http://" OTA_IP "/spiff_rom.bin"


#define RBOOT_SPIFFS_0 0x100000
#define RBOOT_SPIFFS_1 0x300000

#ifndef SPIFF_SIZE
#define SPIFF_SIZE 524288  // 512K
#endif
 
// #define X0_BIN "0x00000.bin"
// #define X9_BIN "0x09000.bin"

extern void alex_init();

/*

#include <inttypes.h>

class Alex
{

public:
	// Initialize
  Alex(uint8_t pin, unsigned long interval_millis ); 
	// Sets the deAlex interval
  void interval(unsigned long interval_millis); 
	// Updates the pin
	// Returns 1 if the state changed
	// Returns 0 if the state did not change
  int update(); 
	// Forces the pin to signal a change (through update()) in X milliseconds 
	// even if the state does not actually change
	// Example: press and hold a button and have it repeat every X milliseconds
  void reAlex(unsigned long interval); 
	// Returns the updated pin state
  int read();
	// Sets the stored pin state
  void write(int new_state);
    // Returns the number of milliseconds the pin has been in the current state
  unsigned long duration();
  // The risingEdge method is true for one scan after the de-Alexd input goes from off-to-on.
	bool risingEdge();
  // The fallingEdge  method it true for one scan after the de-Alexd input goes from on-to-off. 
	bool fallingEdge();
  
protected:
  int deAlex();
  unsigned long  previous_millis, interval_millis, reAlex_millis;
  uint8_t state;
  uint8_t pin;
  uint8_t stateChanged;
}
*/
