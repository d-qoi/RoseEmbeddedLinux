#! /usr/bin/python3

from curses import wrapper
from time import sleep, time
from Adafruit_BBIO import GPIO

p3 = 'GP0_3'
p4 = 'GP0_4'
p5 = 'GP0_5'
p6 = 'GP0_6'

def setupGPIO():
    GPIO.setup(p3, GPIO.IN)
    GPIO.setup(p4, GPIO.IN)
    GPIO.setup(p5, GPIO.IN)
    GPIO.setup(p6, GPIO.IN)
    
def poll_keys_and_pins():
    if not GPIO.input(p3):
        return 'KEY_LEFT'

    if not GPIO.input(p4):
        return 'KEY_RIGHT'
    
    if GPIO.input(p5):
        return 'KEY_DOWN'

    if GPIO.input(p6):
        return 'KEY_UP'

        

def main(stdscr):
    setupGPIO()
    while True:
        print('p3: %d, p4: %d, p5: %d, p6: %d\r'%(GPIO.input(p3), GPIO.input(p4), GPIO.input(p5), GPIO.input(p6)))
        sleep(1)

if __name__ == "__main__":
    wrapper(main)
