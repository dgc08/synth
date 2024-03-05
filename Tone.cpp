#include <Arduino.h>
#include <rom/ets_sys.h>

#include "Tone.h"

Tone::Tone() {
  setFreq(0); // Call setup with frequency 0
}

void Tone::setFreq(int frequency) {
  state = false;
  freq = frequency;
  if (frequency <= 0) {
    activated = false;
    return;
  }
  activated = true;
  last = 0;
  interval = 500000 / frequency;
}

bool Tone::getState(bool allowed, unsigned long microsecs) {
  if ( (last + interval) < microsecs && activated) {
    state = !state;
    last = microsecs;
  }
  return state && allowed;
}
