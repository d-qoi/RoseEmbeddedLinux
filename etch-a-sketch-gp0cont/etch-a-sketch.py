#! /usr/bin/python3

import curses
from curses import wrapper
from time import sleep, time
from Adafruit_BBIO import GPIO

p3 = 'GP0_3'
p4 = 'GP0_4'
p5 = 'GP0_5'
p6 = 'GP0_6'
pause = 'PAUSE'


def setupGPIO():
    GPIO.setup(p3, GPIO.IN)
    GPIO.setup(p4, GPIO.IN)
    GPIO.setup(p5, GPIO.IN)
    GPIO.setup(p6, GPIO.IN)
    GPIO.setup(pause, GPIO.IN)

        
def print_Hello(stdscr):
    stdscr.clear()
    stdscr.addstr(0, 0, "Welcome to Alex Hirschfeld's Etch-a-sketch!")
    stdscr.addstr(2, 0, "GP0_3 is move left, GP0_4 is move right, GP0_5 is move down, GP0_6 is move up")
    stdscr.addstr(3, 0, "'shaking' the cursor left and right will clear the screen")
    stdscr.addstr(4, 0, "The pause button will exit the program")
    stdscr.addstr(6, 0, "Press any key (on the keyboard, not the GPIO buttons) to start drawing!")

    stdscr.getkey()

def main(stdscr):
    print_Hello(stdscr)
    shake_count = []


    def handle_shake():
        nonlocal shake_count
        shake_count.append(time())
        if len(shake_count) >= 4:
            shake_count = shake_count[-4:]
            if (shake_count[3] - shake_count[0]) < 2:
                stdscr.clear()
                stdscr.refresh()
                shake_count = []
                return True
        return False
                
    stdscr.clear()
    curses.curs_set(False)
    maxy, maxx = stdscr.getmaxyx()
    y = int(maxy/2)
    x = int(maxx/2)

    running = True
    last_key = None
    key = None 
    while running:
        if GPIO.input(p6) == 0:
            y = y - 1
            x = x
        elif GPIO.input(p5) == 1:
            y = y + 1
            x = x
        elif GPIO.input(p3) == 0:
            key = p3
            if last_key != key:
                if handle_shake():
                    continue
            last_key = key
            x = x - 1
        elif GPIO.input(p4) == 1:
            key = p4
            if last_key != key:
                if handle_shake():
                    continue
            last_key = key
            x = x + 1
        elif GPIO.input(pause) == 0:
            running = False
        else:
            pass #running = False
        if x < 0:
            x = 0
        if x > maxx:
            x = maxx
        if y < 0:
            y = 0
        if y > maxy:
            y = maxy
        stdscr.addstr(y, x, "X")
        stdscr.refresh()
        sleep(0.05)

if __name__ == "__main__":
    setupGPIO()
    wrapper(main)
