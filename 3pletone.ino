#include "Tone.h"
#include "ToneEvents.h"

#define TONE_OUTPUT_PIN 26
#define TONE_COUNT 10

Tone tones[TONE_COUNT];
bool toneAllowed[TONE_COUNT];


bool checkEvents = true;
int nextEvent = 0;
unsigned long lastEvent = 0;

void setup() {
  pinMode(TONE_OUTPUT_PIN, OUTPUT);
  Serial.begin(115200);
  
  #if SERIAL_MODE
  Serial.println("Serial Mode activated. Awaiting Instructions...");
  #else
  if(MAX_TONE_EVENTS == 0) {
    Serial.println("Serial Mode deactivated, but no Instruction Sequence (aka MIDI Sequence) defined. Will effectively do nothing");
  }
  else {
    Serial.print("Instruction Sequence (aka MIDI Sequence) of length ");
    Serial.println(MAX_TONE_EVENTS);
  }
  #endif
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
#if SERIAL_MODE
  if (Serial.available() > 0) {
    String data_string = Serial.readStringUntil('\n');
    data_string.trim();

    // Split data string and convert to integers
    int comma_index = data_string.indexOf(',');
    int channel = data_string.substring(0, comma_index).toInt();
    int freq = data_string.substring(comma_index + 1).toInt();

    tones[channel].setFreq(freq*TRANSPOSE);
  }
#else
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
 #endif
}
