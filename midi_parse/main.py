from os import chdir

from mido import MidiFile

def midi_to_frequency(note_number):
  return int(2 ** ((note_number - 69) / 12) * 440)

if __name__ == '__main__':
  tempo = 60000000 / int(input("BPM>"))
  #ticks_per_beat = 480
  ticks_per_beat = int(input("Ticks per beat>"))

  chdir("files")
  output_file = input("Output File>")
  if output_file == "":
    output_file = "files/convert.h"

  polyphony = 64
 # sweet_dreams.mid
 # sweet_dreams.h
  #midi_file = MidiFile("test.mid")
  midi_file = MidiFile(input("Path>").replace("%D", "C:\\Users\\dgc\\Downloads"))

  notes = []

  for track in midi_file.tracks:
    for message in track:
      if message.type == "set_tempo" and tempo == 0:
        tempo = message.tempo
        print(tempo)
      elif message.type == "time_signature":
        pass
        #raise Exception
        #ticks_per_beat = message.clocks_per_click * 20
      elif message.type == "note_off" or (message.type == "note_on" and message.velocity == 0):
        notes.append(("off", midi_to_frequency(message.note), int((message.time/ticks_per_beat) * (tempo / 1000) ), 0))
      elif message.type == "note_on":
        print(message)
        notes.append(("on", midi_to_frequency(message.note), int((message.time/ticks_per_beat) * (tempo / 1000) ), message.velocity))
      else:
        print(message)


  array = "ToneEvent toneEvents[] = { "

  running_notes = {}
  free_channels = list(range(polyphony))
  max_channel = 0

  for note in notes:
    if note[0] == "on":
      if len(free_channels) == 0:
        print("All channels are used, ignring note")
        channel = -69
      else:
        channel = free_channels[0]
        free_channels.pop(0)
        array += f"ToneEvent({note[2]}, {channel}, {note[1]}, {note[3]}), "

      running_notes[note[1]] = channel
    elif note[0] == "off":
      channel = running_notes[note[1]]
      del running_notes[note[1]]
      free_channels.append(channel)
      free_channels = sorted(free_channels)
      if channel != -69:
        if channel > max_channel:
          max_channel = channel
        array += f"ToneEvent({note[2]}, {channel}, 0, 0), "

  array += "};"

  with open(output_file, "w") as f:
    f.write(array)

  print("max_channel:", max_channel)