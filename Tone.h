#ifndef TONE_H
#define TONE_H

class Tone {
private:
  int interval;
  unsigned long last;
  bool state;
public:
  Tone();
  void setFreq(float frequency);
  bool getState(unsigned long microsecs);
  bool activated;
  int freq;
};

#endif
