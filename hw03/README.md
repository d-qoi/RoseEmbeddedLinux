# Etch-a-sketch on an 8x8 matrix, and a tmp101 alert!

## Etch-a-sketch
* Written in python with the SMBus and Adafruit_BBIO library
* Uses an 8x8 dual color LED matrix (assuming with address 0x70)
* uses a tmp101 temp sensor (assuming with address 0x48)
* both i2c devices are assumed to be on port 1.
* 4 buttons are needed to be on GP0_3
    * Left is mapped to 3
    * right is mapped to 4
    * down is mapped to 5
    * up is mapped to 6

* to end the program, hit the pause button on the board.

## tempReader.sh
* this is written in bash
* this assumes that the alert pin of a tmp101 sensor is attached to GP1_4
* The address of the tmp101 sensor can be passes in with the -s flag, passing the value in hex.
* the alert temperature is given in Fahrenheit, and can be passed into this application in celcius with -t
    * the default temp is 20c
    
# Comments from Prof. Yoder
# Looks good.  Some comments in your code would help.
# Grade:  10/10
