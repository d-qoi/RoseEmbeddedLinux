#! /usr/bin/python3
import Adafruit_BBIO.GPIO as GPIO

GP1_3 = 'GP1_3'
GP1_4 = 'GP1_4'
on = False;

def turnOn(channel):
    if on:
        GPIO.output(GP1_4, GPIO.LOW)
        on = False
    else:
        GPIO.output(GP1_4, GPIO.HIGH)
        on = True

GPIO.setup(GP1_3, GPIO.IN)
GPIO.setup(GP1_4, GPIO.OUT)

GPIO.add_event_callback(GP1_3, GPIO.BOTH, callback=turnOn)

try:
    while(true):
        pass
except Exception as e:
    print(e)
