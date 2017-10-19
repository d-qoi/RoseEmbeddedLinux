# Etch-a-sketch on i2c display

## Etch-a-sketch on an ili9341 display
* Uses Rotary Encoders on ports 2 and 3 for x and y movement
* Uses Mode and Pause buttons for increasing and decreasin the size of the dot.
* "shaking" the horizontal movement encoder will change the color of the dot to <span style= color:red> red </span> and switch to erase mode
  * In erase mode, the dot will erase color instead of drawing color.
  * "Shaking" the encoder again will switch back to regular drawing mode.
* "shaking" the vertical encoder will randomly change the color the dot leaves behind.
