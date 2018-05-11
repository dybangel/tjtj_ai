import RPi.GPIO as GPIO
import time
import os

BUTTON = 17

GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON, GPIO.IN)

while True:
    state = GPIO.input(BUTTON)
    if state:
        print("off")
    else:
        print("on")
        os.system("/work/mengwa/mengwa.sh") 
    time.sleep(1)
