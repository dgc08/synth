#include <rom/ets_sys.h>

#include "Tone.h"

const int TONE_OUTPUT_PIN = 26;
Tone tone1;
Tone tone2;

bool tone1a;
bool tone2a;

void setup() {
  // ledcAttachPin(uint8_t pin, uint8_t channel);
  pinMode(TONE_OUTPUT_PIN, OUTPUT);
  Serial.begin(115200);

  tone1.setup(220);
  tone2.setup(146);
  tone1a = true;
  tone2a = false;
}

void loop() {
  if (tone1.getState(tone1a) || tone2.getState(tone2a)) {
    digitalWrite(TONE_OUTPUT_PIN, HIGH);
    //Serial.println(1);
  }
  else {
    digitalWrite(TONE_OUTPUT_PIN, LOW);
    //Serial.println(0);
  }

  tone2a = !tone2a;
  tone1a = !tone1a;
  
  /*
  // Plays the middle C scale
  ledcWriteTone(TONE_PWM_CHANNEL, BASE_HZ);
  //ledcWrite(TONE_PWM_CHANNEL, 255);
  ets_delay_us(500000/HZ);
  ledcWrite(TONE_PWM_CHANNEL, 0);
  ets_delay_us(500000/HZ);
  */
}
