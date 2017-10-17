# Etch-a-sketch on an 8x8 matrix with encoders, MMap buttons and leds!

## Encoder-Etch-a-sketch
* Written in python with the RCPY and SMbBus library
* Uses an 8x8 dual color LED matrix (assuming with address 0x70)
* Uses two encoders, on port 2 and port 3.
* To clear the screen, 'shake' the encoder on port 2.
* to end the program, hit the pause button on the board.

## GPIO Thru
* Maps two buttons to two LEDs using mmap
* Uses GPIO0 and GPIO1
* Maps GP0_2 and GP0_3 to Bat 1 and Bat 4
