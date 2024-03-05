#include <rom/ets_sys.h>

#include "Tone.h"
#include "ToneEvent.h"

#define TONE_OUTPUT_PIN 26
#define TONE_COUNT 3

Tone tones[TONE_COUNT];
bool toneAllowed[TONE_COUNT];

#define MAX_TONE_EVENTS 3
ToneEvent toneEvents[MAX_TONE_EVENTS] = {};

void setup() {
  pinMode(TONE_OUTPUT_PIN, OUTPUT);
  Serial.begin(115200);

  
  tones[0].setFreq(147);
  tones[1].setFreq(175);
  tones[2].setFreq(220);
  
}

void loop() {
  for (int j = 0; j < TONE_COUNT; j++) {
    if (tones[j].activated == false) {
      continue;
    }
    toneAllowed[j] = true;
    
    bool anyToneActive = false;

    unsigned long microsecs = micros();
    for (int i = 0; i < TONE_COUNT; i++) {
      if (tones[i].getState(toneAllowed[i], microsecs)) {
        anyToneActive = true;
        break;
      }
    }
  
    if (anyToneActive) {
      digitalWrite(TONE_OUTPUT_PIN, HIGH);
    } else {
      digitalWrite(TONE_OUTPUT_PIN, LOW);
    }
    toneAllowed[j] = false;
  }
}
