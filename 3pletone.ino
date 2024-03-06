#include "Tone.h"
#include "ToneEvents.h"

#define TONE_OUTPUT_PIN 26
#define TONE_COUNT 8

Tone tones[TONE_COUNT];
bool toneAllowed[TONE_COUNT];


bool checkEvents = true;
int nextEvent = 0;
unsigned long lastEvent = 0;

void setup() {
  pinMode(TONE_OUTPUT_PIN, OUTPUT);
  Serial.begin(115200);

  /*
  tones[0].setFreq(147);
  tones[1].setFreq(175);
  tones[2].setFreq(220);
  */
  
}

void loop() {
  for (int j = 0; j < TONE_COUNT; j++) {
    if (tones[j].activated == false && tones[j].freq == 0) {
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

  if (checkEvents && millis() > toneEvents[nextEvent].timestamp + lastEvent) {
    tones[toneEvents[nextEvent].toneNumber].setFreq(toneEvents[nextEvent].frequency*TRANSPOSE);
    nextEvent++;
    #if !ABSOLUTE_TIMES
    lastEvent = millis();
    #endif
    
    if (nextEvent == MAX_TONE_EVENTS) {
      if (LOOP_EVENTS) {
        nextEvent = 0;
        
        #if ABSOLUTE_TIMES
        lastEvent = millis();
        #endif
        
        return;
      }
      checkEvents = false;
    }
  }
}
