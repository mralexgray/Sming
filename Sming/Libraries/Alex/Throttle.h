	

#pragma once

#define   DEBOUNCE_DELAY  100

#define  THROTTLE_V_MIN   1.3
#define  THROTTLE_V_MAX   3.3

#define UInt uint8

#include "Runloop.h"
#include <HardwarePWM.h>


class Control: public Runloop {
	
	public:

		enum Ctrl { Ctrl1Button, CtrlJoystick };

		Control(UInt pinIn, UInt pinOut, float minOut = 0, 
																	 float maxOut = 5,
											               UInt levels = 25,
											              Ctrl controlType = CtrlJoystick) :
		_pinIn(pinIn), _pinOut(pinOut), 
		_minOut(minOut), _maxOut(maxOut),
		_levels(levels), _controlType(controlType) { 
			this->_interval = DEBOUNCE_DELAY;
			// uint8_t pins[ 1] = { D1 }; // List of pins that you want to connect to pwm  4erd
			// HardwarePWM HW_pwm(pins, 1);

			 // _pins[0] = this->_pinOut; // List of pins that you want to connect to pwm  4erd
			// HardwarePWM pppwwwmmm;
			// _pwm = HardwarePWM(pins,1); //  HW_pwm; // new HardwarePWM(_pins,1);
		}

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

	// LEDCLASS* led;

	protected:

		uint8_t pins[ 1] = { D1 };

		HardwarePWM _pwm = HardwarePWM(pins,1);

		uint8_t _pinIn, _pinOut, _level, _levels;
		float _minOut, _maxOut;
// #ifdef ESP866
// 		Ticker _delay;
// #else
// 		AsyncDelay _delay;
// #endif
		Ctrl _controlType;
};

class Throttle : public Control {
		
	public:
		Throttle(UInt pinIn = A0, UInt pinOut = D1, 
					 float minOut = THROTTLE_V_MIN, 
					 float maxOut = THROTTLE_V_MAX, 
					 	 UInt levels = 25, 	
					 	Ctrl controlType = CtrlJoystick)

		: Control(pinIn, pinOut, minOut, maxOut, levels, controlType) { }
		//CLASS_NAME(Throttle);
};

class Horn : public Control {
		
	public:
		Horn(UInt pinIn, 	UInt pinOut, 
				float minOut = 0.0,	float maxOut = VREF,
				UInt 	levels = 5, 	Ctrl 	controlType = CtrlJoystick)
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
