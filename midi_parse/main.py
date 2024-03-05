from mido import MidiFile

def midi_to_frequency(note_number):
  return int(2 ** ((note_number - 69) / 12) * 440)

tempo = 500000
ticks_per_beat = 192
notes = []

midi_file = MidiFile("test.mid")

for track in midi_file.tracks:
  print("TRACK:")
  for message in track:
    if message.type == "set_tempo":
      tempo = message.tempo
    if message.type == "note_on":
      notes.append(("on", midi_to_frequency(message.note), int((message.time/ticks_per_beat) * (tempo / 1000) )))
    if message.type == "note_off":
      notes.append(("off", midi_to_frequency(message.note), int((message.time/ticks_per_beat) * (tempo / 1000) )))

print(notes)

array = "ToneEvent toneEvents[] = {\n"

rinning_notes = {}

for note in notes:

