import serial
import time
import json

with serial.Serial('/dev/ttyACM0', 9600) as ser:

    time.sleep(2)  # wait for the serial connection to initialize

    current_time = time.time()  # get the current time

    message = {
        'time': current_time
    }

    ser.write(json.dumps(message).encode())  # send the time message

    while True:
        # Read and print serial
        print(ser.readline().decode('utf-8'))
