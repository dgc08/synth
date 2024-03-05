#ifndef TONE_H
#define TONE_H

class Tone {
private:
  int interval;
  long long last;
  bool state;
public:
  // No constructor needed for this example
  void setup(int frequency);
  bool getState(bool allowed);
  bool activated;
};

#endif
