#ifndef TONE_EVENT_H
#define TONE_EVENT_H

struct ToneEvent {
  // Timestamp in milliseconds since Arduino startup
  unsigned long timestamp;

  int toneNumber;

  // Frequency of the tone in Hz
  int frequency;

  // Default constructor
  ToneEvent() : timestamp(0), toneNumber(-1), frequency(0) {}

  // Constructor with arguments
  ToneEvent(unsigned long timestamp, int toneNumber, int frequency)
      : timestamp(timestamp), toneNumber(toneNumber), frequency(frequency) {}
};

#endif
