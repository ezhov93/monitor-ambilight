#include "timer.h"

void Timer::setInterval(uint32_t interval) {
  _interval = interval;
  reset();
}

boolean Timer::isReady() {
  if (_enable == false) return false;
  const uint32_t ms = millis();
  if (ms - _timer < _interval) {
    return false;
  } else {
    _timer = ms;
    return true;
  }
}