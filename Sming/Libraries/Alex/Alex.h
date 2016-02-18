
/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 Main code by Thomas O Fredericks
 ReAlex and duration functions contributed by Eric Lowry
 Write function contributed by Jim Schimpf
 risingEdge and fallingEdge contributed by Tom Harkaway
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <SmingCore.h>
#include "Arduino.h"

#include <user_config.h>

#define UPDATE_PIN 0 // GPIO0  aka D3

#define WIFI_SSID "Dickens"
#define WIFI_PWD "dickens1931"
 
#define OTA_IP "10.0.0.201"

extern void alex_init();

#ifndef ROM_0_URL // download urls, set appropriately
#define ROM_0_URL  "http://" OTA_IP "/rom0.bin"
#define ROM_1_URL  "http://" OTA_IP "/rom1.bin"
#define SPIFFS_URL "http://" OTA_IP "/spiff_rom.bin"
#endif

#define      ANALOG_MAX  1023
#define         PWM_MAX  22222 // 255
#define         PWM_MIN  0

// #define min(a,b) ((a)<(b)?(a):(b))
// #define max(a,b) ((a)>(b)?(a):(b))
// #define abs(x) ((x)>0?(x):-(x))
// #define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
// #define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
// #define radians(deg) ((deg)*DEG_TO_RAD)
// #define degrees(rad) ((rad)*RAD_TO_DEG)
// #define sq(x) ((x)*(x))

enum PinState {
#ifdef ESP8266
  OFF = HIGH, ON = LOW,
#else
  OFF = LOW,  ON = HIGH,
#endif
  PWM
};

#if    defined(BUILTIN_LED)
  #define LED1 BUILTIN_LED
#elif  defined(LED_BUILTIN)
  #define LED1 LED_BUILTIN
#elif  defined(ESP8266)
  #define LED1 D0
#else
  #define LED1 13
#endif
#define   LED2 2
// #ifdef ESP8266
#define            VREF  3.3
// #else
// #define            VREF  5.0
// #endif

// extern void Stream_printf_progmem(Print &out, PGM_P format, ...);

#define     PF(...)   Serial.printf(__VA_ARGS__)
#define    PLN(X)       Serial.println(X)
#define     PO(X)       Serial.print(X)

#define  NO_OP(...) ({ __VA_ARGS__; (void)NULL; })
#define F_TO_S(FLOAT)   (char*) ({ char s[7];  dtostrf(FLOAT, 4, 3, s); s; })
#define SAME_C(X,Z)     (strcmp(X,Z) == 0)

#define OFF HIGH
#define ON LOW

#include "Led.h"

/*

class LED  {

  enum Pattern {
    Solid,
    Blinking
  };

  public:
    LED(int ping = LED_BUILTIN) {
      pinMode(_pin =  pin, OUTPUT);
      digitalWrite(_pin, _state = LOW); 
    }
    ~LED() {}
    void on(){ digitalWrite(_pin, _state = HIGH); }
    void off(){ digitalWrite(_pin, _state = LOW); }

    void blink(unsigned int times = 1, unsigned int ms = 1000) {
      _pattern = Blinking;
      procTimer.initializeMs(ms,handle).start();

    }
    void toggle() { _state == HIGH ? off() : on(); }
  protected:

    void handle (){
      if (_pattern == Solid || !_step) return;
      if (_pattern == Blinkiung) toggle();
      _step--;

    }
    Timer _timer;
    unsigned int _speed, _step, _pin;
    int _state;
    Pattern _pattern;
};

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
