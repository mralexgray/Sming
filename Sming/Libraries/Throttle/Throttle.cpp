
#include "Alex.h"
#include "Throttle.h"
// #include  "Arduino.h"
// #include  "SmingCore.h"


#define HALF_OF(X) X/2
#define CENTER  (int) HALF_OF(ANALOG_MAX)
#define THRESH  (int) HALF_OF(CENTER)


 Control::Control(
              UInt       pinIn,
              UInt      pinOut,
              float     minOut,
              float     maxOut,
              UInt      levels,
              Ctrl controlType) :

      _pinIn(pinIn), 
    _pinOut(pinOut), 
    _minOut(minOut), 
    _maxOut(maxOut),
    _levels(levels),
    _controlType(controlType) {  /* this->_interval = DEBOUNCE_DELAY; */ }
  
void Control::begin(){
  if (_initialized) return;

  // static uint8_t pins[ 1] = { _pinIn };
  // HardwarePWM hwpwm(pins,1);
  // _pwm = (void*)&hwpwm;

  pinMode(_pinIn, INPUT);
  PF("Throttle is Ready, girl.\n");//, className());
  _initialized = true;
}

void Control::handle(){

/*  if (hasSerialString()) {
    String *s = getSerialString();
    int newInterval = s->toInt();
    if (newInterval != _interval) setInterval(newInterval);
  }

  if (!_delay.isExpired()) return;
*/
  int reading = analogRead(_pinIn) - CENTER;
  
  // PF("Got reading: %i\n", reading);

  if (abs(reading) <= THRESH) return;
     // PF("reading over threshold: %i\n", reading);

/*
    if (led) led->on();
*/    
    reading > 0 ? ({ ++(*this); _interval = 200; }) // _delay.start(_interval,   AsyncDelay::MILLIS); })
                : ({ --(*this); _interval = 50;  }); // _delay.start(_interval/3, AsyncDelay::MILLIS); });
    
    // if (led) {
      
    //   if (_level == _levels)    led->fadeIn();
    //   else if (_level == 0)     led->fadeOut();
    //   else                      led->blink(_level);
    // }  
  // }
  // else if(_led) _led->off();
  
}


int       Control::getLevel()       { return _level; }
float     Control::getDuty()        { return (float)constrain(_level/(float)_levels, 0.0, 1.0); }
float     Control::getVoltage()     { return (getDuty() * voltageRange()) + _minOut; }
int       Control::getPWM()         { return ((float)(getVoltage() / VREF)) * PWM_MAX; }

// int       Control::getSmingPWM()    { return ((float)(getVoltage() / VREF)) * PWM_MAX; }
float     Control::voltageRange()   { return (float) _maxOut - _minOut; }
void      Control::setLevel(int s)  {

  if (s == _level || s > _levels || s < 0) return; _level = s; 


  // _pwm->analogWrite(_pinOut, getPWM());

  // HW_pwm.analogWrite(PWM_PIN, speed * 1000);
  // analogWrite(_pinOut, getPWM());

  print(); 
}

Control&  Control::operator++()  { setLevel(_level + 1); return *this; }
Control&  Control::operator--()  { setLevel(_level - 1); return *this; }

void  Control::print() {
  
  Serial.printf("%s:%i/%i DUTY:%s%% (rng:%sv) v:%s/%s PWM:%i/%i interval:%i\n",  /** className() **/ "throttle", _level, _levels,
                                                          F_TO_S(getDuty()), F_TO_S(voltageRange()),
                                                          F_TO_S(getVoltage()), F_TO_S(VREF),
                                                          getPWM(), PWM_MAX, _interval); 
}

// static InputDebounce _pedal; // not enabled yet, setup has to be called later
// #include <InputDebounce.h>


// Control::Control(int  pinIn, int pinOut, float minOut, float maxOut,int levels, Ctrl  controlType) {

//     _pinIn = pinIn; _pinOut = pinOut; _maxOut = maxOut, _minOut = minOut; _levels = levels; _controlType = controlType;
//     // _led = NULL;
//     // begin();
// }


  // if (delay_6s.isExpired()) {
  //   Serial.print("     6s delay millis=");
  //   Serial.println(millis());
  //   delay_6s.repeat();
  // }
  
  // if (_controlType == Ctrl1Button) {
  
    // _pedal.process(_previousMillis = now); // callbacks called in context of this function  

    // delay(DEBOUNCE_  );
  
  // } else {



//   setup();
// }
// void Throttle::setup(Ctrl controlType, int pinIn, int pinOut) {
// #ifdef ESP8266
  // set analog pwm to max range 0 -255
// #endif
  // Alex.begin(); 
  // _pinOut = pinOut;
  // _pinIn = pinIn;
  // pinMode(, OUTPUT);

  // if ((_controlType = controlType) == Ctrl1Button ) {

  //   _pedal.registerCallbacks(pressed, released, pressedDuration);
  //   _pedal.setup(_pinIn, DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  //   t = *this;
  // }



/*
void pressedDuration(unsigned long duration);
void released();
void pressed();

void pressed()  { BuiltInLED.on(); }

void released() {

  ++t;
  BuiltInLED.off();
  Serial.println("RELEASED");
}

void pressedDuration(unsigned long duration) {
  if (!(100 % duration)) Serial_printf(" (%i ms) ",duration);
  if (duration > 100 ) --t;
}

static Throttle t;
Throttle::Throttle(Ctrl controlType, int pinIn, int pinOut) {
  _controlType = controlType; _pinIn = pinIn; _pinOut = pinOut;
}
*/
  // if (_speed == MPH_MAX || _speed == MPH_MIN)  BuiltInLED.pulse(1);

