# GPIO Timing with Interrupts, constant polling, and kernel level interrupts

*All of these use GP1 3 as the button, and GP1 4 as the LED* 

### gpio_thru_no_interrupt.c
* Constant polling in c using mmap
* Triggering GP1 4 from GP1 3 takes 400ns

### gpio_thru_interrupt.py
* uses the Adafruit_BBIO.GPIO to set up interrupts on the falling edge of the button signal
* Takes 700us, but this may be due to python.

### gpio_thru_kernel
* Uses kernel libraries with interrupts to trigger the led on the falling edge of the button.
* Triggering GP1 4 takes 400us

