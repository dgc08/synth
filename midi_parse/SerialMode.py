import serial
import ast

from time import sleep

def midi_to_frequency(note_number):
  return int(2 ** ((note_number - 69) / 12) * 440)

def recieve(ser):
  if ser.in_waiting > 0:
    received_data = ser.read(ser.inWaiting()).decode('utf-8')
    print(received_data, end="")

if __name__ == '__main__':
  port = 'COM8'
  baudrate = 115200

  ser = serial.Serial(port, baudrate)
  mode = input("Mode([m]idi device/[p]ure text input/read .h file>")
  if mode == "m":
    pass
  elif mode == "p":
    while True:
      try:
        data = input() + "\n"
      except EOFError:
        ser.close()
        exit()

      recieve(ser)
      ser.write(data.encode())
  else:
    filename = input("file>")
    if filename == "":
      filename = "files/Idol.h"
    with open(filename) as f:
      data = f.read().replace("ToneEvent toneEvents[] = {", "(").replace(", };", ")").replace("ToneEvent", "")
      data = ast.literal_eval(data)
      #print(data)

    for note in data:
      sleep(note[0]/1000)
      #print(f"{note[1]},{note[2]}")
      ser.write(f"{note[1]},{note[2]}\n".encode())

      recieve(ser)

    ser.close()
