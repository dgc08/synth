#include <Arduino.h>
#include <rom/ets_sys.h>

#include "Tone.h"

// No implementation needed for the default constructor

void Tone::setup(int frequency) {
  state = false;
  if (frequency == 0) {
    activated = false;
    return;
  }
  activated = true;
  last = 0;
  interval = 500000 / frequency;
}

bool Tone::getState(bool allowed) {
  if ( (last + interval) < micros() && activated) {
    state = !state;
    last = micros();
  }
  return state && allowed;
}
