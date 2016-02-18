
#pragma once

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

