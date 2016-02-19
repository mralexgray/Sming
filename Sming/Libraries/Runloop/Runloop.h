
#pragma once

#include "SmingCore.h"
#include "Arduino.h"

#define OFF HIGH
#define ON LOW

class Runloop {

  public:
    Runloop(){ 
      _timer.initializeUs(_interval, TimerDelegate(&Runloop::handle, this)).start();

    } 
    virtual bool setTimer(int interval) { return interval <= 0 ? false : (_interval = interval); }
    virtual void handle () { }
  protected:
    Timer _timer;
    unsigned int _interval = 500;
    bool _initialized = false;
};

class Led : public Runloop {

  public:

  Led(int p) : _pin(p) { pinMode(_pin, OUTPUT); };
  
  void blink(bool blinking) { if (!blinking)  return _timer.stop();
  
    _timer.initializeMs(_interval, TimerDelegate(&Led::toggle, this)).start();
  }
  void toggle() { digitalWrite(_pin, _state = !_state); }
  void off() { if (_state == OFF) return; _timer.stop(); digitalWrite(_pin, _state = OFF); }
  void on() { if (_state == ON) return; _timer.stop(); digitalWrite(_pin, _state = ON); }

  void onFor(unsigned int duration) {
    on();

    _timer.initializeMs(duration, TimerDelegate(&Led::off, this)).start();
  }
  private:
    int  _state = OFF, _pin = 2;  
};

#define LEDPIN_1 BUILTIN_LED  // GPIO2
#define LEDPIN_2 D8           // GPIO4

