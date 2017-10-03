#! /usr/bin/env python3

from time import time, sleep
from math import floor

import rcpy 
import rcpy.encoder as encoder
import smbus

from Adafruit_BBIO import GPIO

pause = "PAUSE"
matrix = 0x70

matrix_data = [0]*16

shake_count = []

bus = smbus.SMBus(1)

def setup():
    GPIO.setup(pause, GPIO.IN)
    
    bus.write_byte_data(matrix, 0x21, 0)
    bus.write_byte_data(matrix, 0x81, 0)
    bus.write_byte_data(matrix, 0xef, 0)
    bus.write_i2c_block_data(matrix, 0, [0]*16)
    
    rcpy.set_state(rcpy.RUNNING)
    
def dismantle():
    GPIO.cleanup(pause)
    bus.write_i2c_block_data(matrix, 0, [0]*16)
    
def write_to_matrix(row, col):
    for i in range(0, 16, 2):
        if matrix_data[i] > 0:
            matrix_data[i+1] |= matrix_data[i]
            matrix_data[i] = 0
    matrix_data[col*2] = 1<<row
    
def handle_shake():
    global shake_count, matrix_data
    shake_count.append(time())
    if len(shake_count) > 4:
        shake_count = shake_count[-4:]
        if (shake_count[3] - shake_count[0]) < 1:
            matrix_data = [0]*16
            shake_count = []
            return True
    return False
        
        
def enc_to_screen(enc):
    encRange = 200
    scrRange = 8
    ratio = scrRange/encRange
    
    enc += 100
    
    return floor(enc*ratio)

def etchAsketch():
    global matrix_data

    running = True
    last_key = 1
    key = 0 
    while running:
        
        x = encoder.get(2)
        y = encoder.get(3)
        
        x = enc_to_screen(x)
        y = enc_to_screen(y)
        
        if GPIO.input(pause) == 0:
            running = False
        else:
            pass #running = False
        if x < 0:
            x = 0
        if x > 7:
            x = 7
        if y < 0:
            y = 0
        if y > 7:
            y = 7
            
        #print(x, key, last_key)
        
        if x - key > 0 and last_key < 0:
            last_key = 1
            handle_shake()
        elif x - key < 0 and last_key > 0:
            last_key = -1
            handle_shake()
            
        key = x
        
        write_to_matrix(y, x)
        # print(matrix_data)
        # print(x, y)
        bus.write_i2c_block_data(matrix, 0, matrix_data)
        sleep(0.1)
    
def main():
    print("""
        This program uses 2 encoder wheels attached to port 2 and 3, and an LED matrix on the i2c port with address 0x70.
        
        The encoder on port 2 moves in the x direction and port 3 moves in the y direction.
        
        "shaking" the x encoder by rotating it quickly, will clear the screen.
        
        Pressing the Pause button will exit this program.
    """)
    
    setup()
    bus.write_byte_data(matrix, 0xef, 0)
    try:
        etchAsketch()
    except Exception as e:
        pass
    finally:
        dismantle()
    
    print("""
    
    
Thanks for playing!
    """)
    
if __name__ == '__main__':
    main()
