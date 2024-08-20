# ESP-32 Synthesizer
Square-wave synth with multiple channels for the ESP-32. There are no libraries used, just the `ledc`-functions from the ESP-32 standart library to set the PWM output for the speaker.

You can either hard-code some MIDI notes or send them over Serial, which enables this to also work as output device for MIDI notes from a MIDI keyboard.

[Small demonstration](https://www.youtube.com/watch?v=hLQV4Rru0rQ)
