from threading import Thread

import serial
import ast

from os import chdir
from time import sleep
import mido

polyphony = 10
user_input_thread = None

class FakeDevice:
  def __init__(self):
    self.msgs = []
    self.tempo = 60000000 / int(input("BPM>"))
    # ticks_per_beat = 480
    self.ticks_per_beat = int(input("Ticks per beat>"))

    chdir("files")

    midi_file = mido.MidiFile(input("Path>").replace("%D", "C:\\Users\\dgc\\Downloads"))

    for track in midi_file.tracks:
      for message in track:
        self.msgs.append(message)


  def receive(self):
    message = self.msgs[0]
    self.msgs.pop(0)
    sleep(int((message.time/self.ticks_per_beat) * (self.tempo / 1000) ) / 1000)
    message.time = 0
    return message

def midi_to_frequency(note_number):
  return int(2 ** ((note_number - 69) / 12) * 440)

def recieve(ser):
  if ser.in_waiting > 0:
    received_data = ser.read(ser.inWaiting()).decode('utf-8')
    print(received_data, end="")

def user_send(ser):
  global user_input_thread
  def get_and_send_user_input(ser):
    """Continuously gets user input and sends it to the serial port with a newline."""
    while True:
      try:
        user_input = input()
        user_input_with_newline = user_input + "\n"
        ser.write(user_input_with_newline.encode())  # Encode for serial communication
      except (KeyboardInterrupt, serial.SerialException) as e:
        print(f"Error: {e}")
        break  # Exit the thread loop on errors

  user_input_thread = Thread(target=get_and_send_user_input, args=(ser,))
  user_input_thread.start()

def device_play(device, free_channels, running_notes):
  recieve(ser)
  user_send(ser)
  message = device.receive()

  channel = None
  if message.type == "note_on" and message.velocity > 0:
    if len(free_channels) == 0:
      print("All channels are used, ignring note")
    else:
      channel = free_channels[0]
      free_channels.pop(0)
      running_notes[message.note] = channel
  elif (message.type == "note_on" and message.velocity == 0) or message.type == "note_off":
    try:
      channel = running_notes[message.note]
      del running_notes[message.note]
      free_channels.append(channel)
      free_channels = sorted(free_channels)
    except KeyError:
      pass
  if channel != None:
    ser.write(f"{channel},{midi_to_frequency(message.note)},{message.velocity}\n".encode())


if __name__ == '__main__':
  port = 'COM8'
  baudrate = 115200

  ser = serial.Serial(port, baudrate)
  mode = input("Mode([m]idi device/[p]ure text input/[r]ead .mid file/read .h file>")
  if mode == "m":
    device = mido.open_input(mido.get_input_names()[0])

    free_channels = list(range(polyphony))
    running_notes = {}
    while True:
      device_play(device, free_channels, running_notes)
  elif mode == "r":
    device = FakeDevice()

    free_channels = list(range(polyphony))
    running_notes = {}
    while True:
      device_play(device, free_channels, running_notes)

  elif mode == "p":
    while True:
      recieve(ser)
      user_send(ser)
      try:
        data = input() + "\n"
      except EOFError:
        ser.close()
        exit()

      ser.write(data.encode())
  else:
    chdir("files")
    filename = input("file>")
    if filename == "":
      filename = "Idol.h"
    with open(filename) as f:
      data = f.read().replace("ToneEvent toneEvents[] = {", "(").replace(", };", ")").replace("ToneEvent", "")
      data = ast.literal_eval(data)
      #print(data)

    for note in data:
      sleep(note[0]/1000)
      #print(f"{note[1]},{note[2]}")
      if len(note) == 4:
        ser.write(f"{note[1]},{note[2]},{note[3]}\n".encode())
      else:
        ser.write(f"{note[1]},{note[2]}\n".encode())

      recieve(ser)
      user_send(ser)

  ser.close()
  user_input_thread.join()