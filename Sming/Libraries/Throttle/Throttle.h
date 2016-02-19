	

#pragma once

#include "SmingCore.h"
#include "Runloop.h"
#include "HardwarePWM.h"

#define   DEBOUNCE_DELAY  100

#define  THROTTLE_V_MIN   1.3
#define  THROTTLE_V_MAX   3.3

#define DEF_IN_PIN A0
#define DEF_PWM_PIN 13

#define UInt uint8

#define      ANALOG_MAX  1023
#define         PWM_MAX  22222 // 255
#define         PWM_MIN  0


// enum PinState {
//   OFF = HIGH, 
//   ON = LOW,
//   PWM
// };

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


class Control: public Runloop {
	
	public:

		enum Ctrl { Ctrl1Button, CtrlJoystick };


		Control(	
							UInt   		 pinIn = DEF_IN_PIN,
							UInt  		pinOut = DEF_PWM_PIN,
							float 		minOut = 0.0,
							float 		maxOut = VREF,
							UInt  		levels = 25,
						  Ctrl controlType = CtrlJoystick);

		virtual ~Control(){}

		void begin();
		void handle();

		float 	   getDuty();
		float   getVoltage();
		float voltageRange();
		int         getPWM();
		void   setLevel(int);
		int       getLevel();
	
		void print();

		Control& operator++();
		Control& operator--();

		// HardwarePWM *_pwm;

	protected:

		// uint8_t pins[ 1] = { _pinIn };
		// HardwarePWM _pwm = HardwarePWM(pins,1);

		uint8_t _pinIn, _pinOut, _level, _levels;
		float _minOut, _maxOut;
		Ctrl _controlType;
};


class Throttle : public Control {
		
	// public:
	// 	Throttle(UInt pinIn, UInt pinOut,
	// 				 float minOut, 
	// 				 float maxOut, 
	// 				 UInt levels = 25,	Ctrl controlType = CtrlJoystick)

	// 	: Control(pinIn, pinOut, minOut, maxOut, levels, controlType) { }
		//CLASS_NAME(Throttle);
};

class Horn : public Control {
		
	public:
		Horn(UInt pinIn, 	UInt pinOut,
				float minOut,	float maxOut,
				UInt levels = 5, Ctrl 	controlType = CtrlJoystick)
		: Control(pinIn,pinOut, minOut,maxOut,levels,controlType) {  }
		//CLASS_NAME(Horn);
};


/*					float minOut = THROTTLE_V_MIN, 				float maxOut = THROTTLE_V_MAX,
					int 	levels = 25, 				Ctrl 	controlType = CtrlJoystick);
	
	void setup(Ctrl, int pinIn, int pinOut);
	void    setSpeed(int);
	int     getSpeed();
#define         MPH_MIN  0
#define         MPH_MAX  25
#define      DEFAULT_IN  A0
#if defined(ESP_H)
#define     DEFAULT_OUT  D2
#else
#define     DEFAULT_OUT  6
#endif
*/


			// uint8_t pins[ 1] = { D1 }; // List of pins that you want to connect to pwm  4erd
			// HardwarePWM HW_pwm(pins, 1);

			 // _pins[0] = this->_pinOut; // List of pins that you want to connect to pwm  4erd
			// HardwarePWM pppwwwmmm;
			// _pwm = HardwarePWM(pins,1); //  HW_pwm; // new HardwarePWM(_pins,1);
	// LEDCLASS* led;

// #ifdef ESP866
// 		Ticker _delay;
// #else
// 		AsyncDelay _delay;
// #endif
