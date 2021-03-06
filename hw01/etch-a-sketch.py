#! /usr/bin/python3

import curses
from curses import wrapper
from time import time

def print_Hello(stdscr):
    stdscr.clear()
    stdscr.addstr(0, 0, "Welcome to Alex Hirschfeld's Etch-a-sketch!")
    stdscr.addstr(2, 0, "Use the cursor keys to move the cursor around.")
    stdscr.addstr(3, 0, "'shaking' the cursor left and right will clear the screen")
    stdscr.addstr(4, 0, "Any other key will exit the program")
    stdscr.addstr(6, 0, "Press any key to start drawing!")

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
    
    while running:
        stdscr.addstr(y, x, "X")
        key = stdscr.getkey()
        if key == "KEY_UP":
            y = y - 1
            x = x
        elif key == "KEY_DOWN":
            y = y + 1
            x = x
        elif key == "KEY_LEFT":
            if last_key != key:
                if handle_shake():
                    continue
            last_key = key
            x = x - 1
        elif key == "KEY_RIGHT":
            if last_key != key:
                if handle_shake():
                    continue
            last_key = key
            x = x + 1
        else:
            running = False
        if x < 0:
            x = 0
        if x > maxx:
            x = maxx
        if y < 0:
            y = 0
        if y > maxy:
            y = maxy

if __name__ == "__main__":
    wrapper(main)