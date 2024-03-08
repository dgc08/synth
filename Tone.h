#ifndef TONE_H
#define TONE_H

class Tone {
private:
  int interval;
  unsigned long last;
  bool state;
public:
  Tone();
  char velo;
  void setFreq(float frequency, char velocity = 127);
  bool getState(unsigned long microsecs);
  bool activated;
  int freq;
};

#endif
