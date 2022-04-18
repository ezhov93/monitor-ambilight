#pragma once

#include <Arduino.h>

class Timer {
 private:
  uint32_t _timer;
  uint32_t _interval;
  boolean _enable;

 public:
  Timer(uint32_t interval = 1000) {
    setInterval(interval);
    _enable = false;
  }
  void setInterval(uint32_t interval);
  boolean isReady();
  void reset() { _timer = millis(); }
  void stop() { _enable = false; }
  void start() {
    reset();
    _enable = true;
  }
};