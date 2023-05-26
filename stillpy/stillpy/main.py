import logging

import serial
import time
import json
import stillpy

logging.info("Starting up")
with serial.Serial('/dev/ttyACM0', 9600) as ser:
    logging.info("Serial connection established")
    time.sleep(2)  # wait for the serial connection to initialize

    current_time = time.time()  # get the current time

    message = {
        'time': int(current_time)
    }
    logging.debug("Sending message: %s", message)
    ser.write(json.dumps(message).encode())  # send the time message

    logging.info("Reading from serial...")
    while True:
        # Read and print serial
        msg = ser.readline().decode('utf-8')
        logging.info(msg)
