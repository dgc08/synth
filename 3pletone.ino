#include "Tone.h"
#include "ToneEvents.h"

#define TONE_OUTPUT_PIN 26
#define POLYPHONY 10

#define COMBINER_GAIN 1

Tone tones[POLYPHONY];

bool checkEvents = true;
int nextEvent = 0;
unsigned long lastEvent = 0;


void setup() {
  
  ledcSetup(0, 48000, 8);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(TONE_OUTPUT_PIN, 0);

  
  Serial.begin(115200);

  Serial.print("Starting Synth with polyphony ");
  Serial.println(POLYPHONY);
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
  float net = 0;
  int active = 0;
  float duty = 0;
  unsigned long mic = micros();
  for (int i = 0; i < POLYPHONY; i++) {
    if (!tones[i].activated) {
      continue;
    }
    active++;
    if (tones[i].getState(mic)) {
      net += 1;
    }
    else {
      net -= 1;
    }
  }
  
  net += active; // net is all the values of the waves added. this is in the range of -active to +active, add active to make it range 0 to +active*2
  //net *= pow(POLYPHONY/active, 0.9);
  if (active > 0) { // If more than one is active 
    duty = (net * 255 * 0.5) / (POLYPHONY * (1/COMBINER_GAIN));
  }

  ledcWrite(0, duty);
  
#if SERIAL_MODE
  if (Serial.available() > 0) {
    String data_string = Serial.readStringUntil('\n');
    data_string.trim();

    // Split data string and convert to integers
    int comma_index = data_string.indexOf(',');
    int channel = data_string.substring(0, comma_index).toInt();
    float freq = data_string.substring(comma_index + 1).toFloat();
    
    if (channel >= 0) {
      tones[channel].setFreq(freq*TRANSPOSE);
    }
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
