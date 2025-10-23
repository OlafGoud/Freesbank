import serial
import time

# --- Configure these for your cnc board ---
PORT = 'COM3'      # e.g., Windows: 'COM3', Linux/Mac: '/dev/ttyUSB0'
BAUDRATE = 9600  # Default for cnc

# Open serial connection
with serial.Serial(PORT, BAUDRATE, timeout=1) as cnc:
  time.sleep(2)  # Wait for cnc to initialize
  cnc.reset_input_buffer()

  print("Connected to cnc. Type commands or 'exit' to quit.")
  print("Example: send '?' to see machine position.\n")

  while True:
    cmd = input(">> ").strip()
    if cmd.lower() == 'exit':
      break

    # Send command followed by newline
    cnc.write((cmd + '\n').encode())

    # Give cnc a moment to respond
    time.sleep(0.1)

    # Read and print available response
    while cnc.in_waiting > 0:
      response = cnc.readline().decode(errors='ignore').strip()
      print(response)
