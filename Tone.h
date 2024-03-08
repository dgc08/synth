#ifndef TONE_H
#define TONE_H

class Tone {
private:
  int interval;
  unsigned long last;
  int state;
  char velo;
public:
  Tone();
  void setFreq(float frequency, char velocity = 127);
  double getState(unsigned long microsecs);
  bool activated;
  int freq;
};

#endif
