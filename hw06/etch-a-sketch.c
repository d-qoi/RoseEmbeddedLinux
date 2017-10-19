/*
To test that the Linux framebuffer is set up correctly, and that the device permissions
are correct, use the program below which opens the frame buffer and draws a gradient-
filled red square:

retrieved from:
Testing the Linux Framebuffer for Qtopia Core (qt4-x11-4.2.2)

http://cep.xor.aps.anl.gov/software/qt4-x11-4.2.2/qtopiacore-testingframebuffer.html
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <time.h>

#include "/opt/source/Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "/opt/source/Robotics_Cape_Installer/libraries/roboticscape.h"
//#include "beaglebone_gpio.h"

#define GPIO2_START_ADDR 0x481AC000
#define GPIO2_END_ADDR 0x481AD000
#define GPIO2_SIZE (GPIO2_END_ADDR - GPIO2_START_ADDR)

#define PAUSE (1<<5)
#define MODE (1<<4)

#define GPIO_OE 0x134
#define GPIO_DATAIN 0x138


int shake(time_t timeArray[], unsigned int len) {
    int i = 0;
    // loop through everything and move them down one, if the last value isn't a 0
    if (timeArray[len-1] != 0) {
        for (i = 1; i<len; i++) {
            timeArray[i-1] = timeArray[i];       
        }
        timeArray[len-1] = 0;
    }
    // set the first zero to a time, and break, will always be at least 1 zero
    for(i = 0; i<len; i++) {
        if (timeArray[i] == 0) {
            timeArray[i] = time(NULL);
            break;
        }
    }
    // return if all less than 2 seconds.
    if (i == len-1 && timeArray[i] - timeArray[0] < 1) { // If everything is less than 1 second.
        for (i = 0; i<len; i++) {
            timeArray[i] = 0;
        }
        return 1;
    }
    return 0;
    
}

void x_shakehandle(int* erase) {
    if (*erase != 0) {
        *erase = 0;
    } else {
        *erase = 1;
    }
}

void y_shakehandle(int* r, int* g, int* b) {
    *r = rand()%0x1F;
    *g = rand()%0x3F;
    *b = rand()%0x1F;
}

int main()
{
    printf("Starting\n");
    int fbfd = 0;
    int memfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 1;       // Make it so the it runs before the encoder is moved
    int xold = 0, yold = 0;
    long int location = 0;
    
    volatile void *gpio2_addr;
    volatile unsigned int *gpio_oe_addr;
    volatile unsigned int *gpio2_data;
    unsigned int reg;
    
    int size = 3;
    int size_alt = 0;
    
    int r = 0;     // 5 bits
    int g = 17;      // 6 bits
    int b = 0;      // 5 bits
    
    int erase = 0;
    
    
    const int timeArraySize = 7;
    time_t* timeArrayX[timeArraySize];
    time_t* timeArrayY[timeArraySize];
    int xdir = 0;
    int ydir = 0;
    for(int i = 0; i<timeArraySize; i++) {
        timeArrayX[i] = 0;
        timeArrayY[i] = 0;
    }
    
    memfd = open("/dev/mem", O_RDWR);
    printf("Opened def/mem\n");
    
    gpio2_addr = (unsigned int*)mmap(0, GPIO2_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, 
                        GPIO2_START_ADDR);
    if(gpio2_addr == MAP_FAILED) {
        printf("Unable to map GPIO2\n");
        exit(1);
    }
    printf("mapped GPIO2 successfully\n");
    gpio_oe_addr = gpio2_addr + GPIO_OE;
    // puts("1");
    // printf("%#08x %#08x\n",gpio2_addr, gpio_oe_addr);
    reg = *gpio_oe_addr;
    // puts("2");
    reg |= (PAUSE | MODE);
    // puts("3");
    *gpio_oe_addr = reg;
    // puts("4");
    gpio2_data = gpio2_addr + GPIO_DATAIN;
    // puts("5");

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    printf("Offset: %dx%d, line_length: %d\n", vinfo.xoffset, vinfo.yoffset, finfo.line_length);
    
    if (vinfo.bits_per_pixel != 16) {
        printf("Can't handle %d bpp, can only do 16.\n", vinfo.bits_per_pixel);
        exit(5);
    }

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    // initialize hardware first
	if(rc_initialize()){
		fprintf(stderr,"ERROR: failed to run rc_initialize(), are you root?\n");
		return -1;
	}

	printf("\nRaw encoder positions\n");
	printf("   E1   |");
	printf("   E2   |");
	printf("   E3   |");
	printf("   E4   |");
	printf(" \n");
	
	// Black out the screen
	short color = (0<<11) | (0 << 5) | 8;  // RGB
	for(int i=0; i<screensize; i+=2) {
	    fbp[i  ] = color;      // Lower 8 bits
	    fbp[i+1] = color>>8;   // Upper 8 bits
	}

	while(rc_get_state() != EXITING) {
		printf("\r");
		for(int i=1; i<=4; i++){
			printf("%6d  |", rc_get_encoder_pos(i));
		}
		fflush(stdout);
        // Update framebuffer
        // Figure out where in memory to put the pixel
        x = (rc_get_encoder_pos(1)/2 + vinfo.xres) % vinfo.xres;
        y = (rc_get_encoder_pos(3)/2 + vinfo.yres) % vinfo.yres;
        // printf("xpos: %d, xres: %d\n", rc_get_encoder_pos(1), vinfo.xres);
        
        // get GPIO data
        // will set it to 1 if pause is pressed, -1 if mode is pressed, 0 if both or neither.
        // will require user to press button multiple times to increase size.
        if (size_alt != 0) {
            size_alt = ((*gpio2_data >> 5)&0x1) - ((*gpio2_data >> 4)&0x1);
        } else {
            size_alt = ((*gpio2_data >> 5)&0x1) - ((*gpio2_data >> 4)&0x1);
            size += size_alt;
            if (size < 1)
                size = 1;
        }
        
        
        //printf("%d %d %d, %d %d %d\n", x, xold, xdir, y, yold, ydir);
        if (x > xold && xdir <= 0) {
            xdir = 1;
            if (shake(timeArrayX, timeArraySize)) {
                x_shakehandle(&erase);
            }
        } else if(x < xold && xdir >= 0) {
            xdir = -1;
            if (shake(timeArrayX, timeArraySize)) {
                x_shakehandle(&erase);
            }
        }
        if (y > yold && ydir <= 0) {
            ydir = 1;
            if(shake(timeArrayY, timeArraySize)) {
                y_shakehandle(&r, &g, &b);
            }
        } else if(y < yold && ydir >= 0) {
            ydir = -1;
            if(shake(timeArrayY, timeArraySize)) {
                y_shakehandle(&r, &g, &b);
            }
        }
        
        
        if((x != xold) || (y != yold)) {
            printf("Updating location to %d, %d\n", x, y);
            // Set old location to green
            for (int i = -size; i<size; i++) {
                for (int j = -size; j<size; j++) {
                    // add res then mod res, will force it into the resolution area.
                    location = ((xold+i+vinfo.xres)%vinfo.xres+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       ((yold+j+vinfo.yres)%vinfo.yres+vinfo.yoffset) * finfo.line_length;
                    unsigned short int t = r<<11 | g << 5 | b;
                    if (!erase)
                        *((unsigned short int*)(fbp + location)) = t;
                    else
                        *((unsigned short int*)(fbp + location)) = (unsigned short int)8;
                    
                    
                    // Set new location to white
                    //location = ((x+i+vinfo.xres)%vinfo.xres+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    //           ((y+j+vinfo.yres)%vinfo.yres+vinfo.yoffset) * finfo.line_length;
            
                    //*((unsigned short int*)(fbp + location)) = 0xff;
                }
            }
            
            xold = x;
            yold = y;
        }
        
        for (int i = -size; i<size; i++) {
            for (int j = -size; j<size; j++) {
                location = ((x+i+vinfo.xres)%vinfo.xres+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   ((y+j+vinfo.yres)%vinfo.yres+vinfo.yoffset) * finfo.line_length;
                
                *((unsigned short int*)(fbp + location)) = (!erase) ? 0xff : 0b11111000000000000;
            }
        }
		
		rc_usleep(5000);
	}
	
	rc_cleanup();
    
    munmap(fbp, screensize);
    munmap((void *)gpio2_addr, GPIO2_SIZE);
    close(fbfd);
    close(memfd);
    return 0;
}
