#pragma once
#include <Arduino.h>

#include "timer.h"

class Routine {
 private:
  Timer _timer;
  void (*_callback)();

 public:
  static constexpr uint32_t Everytime = 0;
  Routine(uint32_t period, void (*callback)())
      : _timer(period), _callback(callback) {}
  void run() { _timer.start(); }
  void terminate() { _timer.stop(); }
  void process() {
    if (_timer.isReady() == true && _callback != nullptr) _callback();
  }
};
