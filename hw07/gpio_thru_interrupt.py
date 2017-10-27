#! /usr/bin/python3
import Adafruit_BBIO.GPIO as GPIO
import time

GP1_3 = 'GP1_3'
GP1_4 = 'GP1_4'
on = False;

def turnOn(channel):
    global on
    if on:
        GPIO.output(GP1_4, GPIO.LOW)
        on = False
    else:
        GPIO.output(GP1_4, GPIO.HIGH)
        on = True

GPIO.setup(GP1_3, GPIO.IN)
GPIO.setup(GP1_4, GPIO.OUT)

GPIO.add_event_detect(GP1_3, GPIO.BOTH, callback=turnOn)

try:
    while(True):
        time.sleep(100)
except Exception as e:
    print(e)
