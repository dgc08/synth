#include <Arduino.h>
#include <rom/ets_sys.h>

#include "Tone.h"

Tone::Tone() {
  setFreq(0); // Call setup with frequency 0
}

void Tone::setFreq(float frequency, char velocity) {
  if (velocity > 127) {
    velocity = 127;
  }
  state = 1;
  freq = frequency;
  velo = velocity;
  if (frequency <= 0 || velocity <= 0) {
    activated = false;
    return;
  }
  activated = true;
  last = micros();
  interval = 500000 / frequency;
}

double Tone::getState(unsigned long microsecs) {
  if ( (last + interval) < microsecs && activated) {
    state = -1*state;
    last = microsecs;
  }

  float value = state * (velo / 127.0);
  
  return value;
}
