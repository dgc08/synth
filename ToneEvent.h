struct ToneEvent {
  // Timestamp in milliseconds since Arduino startup
  unsigned long timestamp;

  int toneNumber;

  // Frequency of the tone in Hz
  int frequency;

  // Velocity of the tone (default: maximum volume)
  unsigned char velocity = 127;

  // Default constructor
  ToneEvent() : timestamp(0), toneNumber(-1), frequency(0), velocity(127) {}

  // Constructor with arguments
  ToneEvent(unsigned long timestamp, int toneNumber, int frequency, unsigned char velocity)
      : timestamp(timestamp), toneNumber(toneNumber), frequency(frequency), velocity(velocity) {}
};
