#ifndef TONE_H
#define TONE_H

class Tone {
private:
  int interval;
  unsigned long last;
  bool state;
public:
  Tone();
  void setFreq(int frequency);
  bool getState(bool allowed, unsigned long microsecs);
  bool activated;
};

#endif
