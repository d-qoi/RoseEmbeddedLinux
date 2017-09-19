import smbus
from time import time, sleep

from Adafruit_BBIO import GPIO



p3 = 'GP0_3'
p4 = 'GP0_4'
p5 = 'GP0_5'
p6 = 'GP0_6'
pause = 'PAUSE'

bus = smbus.SMBus(1)

tmp101 = 0x48
matrix = 0x70

matrix_data = [0]*16

start_temp = bus.read_byte_data(tmp101, 0)
start_brightness = 0xe5

shake_count = []

def setup():
    GPIO.setup(p3, GPIO.IN)
    GPIO.setup(p4, GPIO.IN)
    GPIO.setup(p5, GPIO.IN)
    GPIO.setup(p6, GPIO.IN)
    GPIO.setup(pause, GPIO.IN)
    
    bus.write_byte_data(matrix, 0x21, 0)
    bus.write_byte_data(matrix, 0x81, 0)
    bus.write_byte_data(matrix, 0xef, 0)
    bus.write_i2c_block_data(matrix, 0, [0]*16)
    
def dismantle():
    GPIO.cleanup(p3)
    GPIO.cleanup(p4)
    GPIO.cleanup(p5)
    GPIO.cleanup(p6)
    GPIO.cleanup(pause)
    
    bus.write_i2c_block_data(matrix, 0, [0]*16)
    
def write_to_matrix(row, col):
    for i in range(0, 16, 2):
        if matrix_data[i] > 0:
            matrix_data[i+1] |= matrix_data[i]
            matrix_data[i] = 0
    matrix_data[col*2] = 1<<row
    
def get_temp():
    return bus.read_byte_data(tmp101, 0)

def get_brightness():
    return get_temp()-start_temp + start_brightness
    
    
def handle_shake():
    global shake_count, matrix_data
    shake_count.append(time())
    if len(shake_count) > 4:
        shake_count = shake_count[-4:]
        if (shake_count[3] - shake_count[0]) < 2:
            matrix_data = [0]*16
            shake_count = []
            return True
    return False
        
def etchAsketch():
    global matrix_data
    
    maxy = 7
    maxx = 7
    
    y = int(maxy/2)
    x = int(maxx/2)

    running = True
    last_key = None
    key = None 
    while running:
        if GPIO.input(p6) == 1:
            y = y + 1
            x = x
        elif GPIO.input(p5) == 0:
            y = y - 1
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
            
        write_to_matrix(y, x)
        # print(matrix_data)
        # print(x, y)
        ilum = get_brightness()
        if ilum < 0xe0:
            ilum = 0xe0
            bus.write_i2c_block_data(matrix, 0, matrix_data)
            bus.write_byte_data(matrix, ilum, 0)
        elif ilum > 0xe9:
            ilum = 0xef
            matrix_data = [0]*16
            bus.write_i2c_block_data(matrix, 0, [0xFF, 0]*8)
            bus.write_byte_data(matrix, ilum, 0)
        else:
            bus.write_i2c_block_data(matrix, 0, matrix_data)
            bus.write_byte_data(matrix, ilum, 0)
        sleep(0.1)
    
def main():
    print("""
Setup::
This assumes that there is a temp sensor and an 8x8 LED matrix attached to i2c 
bus 1, the matrix's address is 0x70 and the tmp101 sensor's address is 0x48


Directions::

This also assumes that: 
    GP0_3 is left
    GP0_4 is right
    GP0_5 is down
    GP0_6 is up
    
If left and right are pressed 5 times in quick succession, it wil clear the screen.

If the temp sensor reads 5c above the temperature at startup, it will clear the screen
If the temp sensor reads 5c less than the temp at startup, the screen will be disabled, but not cleared.If

Enjoy!

Alexander Hirschfeld
    """)
    
    setup()
    bus.write_byte_data(matrix, get_brightness(), 0)
    etchAsketch()
    dismantle()
    
    print("""
    
    
Thanks for playing!
    """)
    
if __name__ == '__main__':
    main()