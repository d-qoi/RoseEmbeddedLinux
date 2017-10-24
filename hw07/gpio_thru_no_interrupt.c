
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>

#include "beaglebone_gpio.h"

int cont = 1;

void signal_handler(int);

void signal_handler(int sig) {
    printf("Captured ^c\n");
    cont = 0;
}

int main(int argc, char* argv[]) {
    volatile void* gpio3_addr;
    
    volatile unsigned int* gpio_oe_addr;
    volatile unsigned int* gpio_datain;
    volatile unsigned int* gpio_set_dataout;
    volatile unsigned int* gpio_clear_dataout;
    
    volatile unsigned int reg;
    
    signal(SIGINT, signal_handler);
    
    printf("%d\n", O_RDWR);
    
    int fd = open("/dev/mem", O_RDWR);
    if (fd == 0) {
        printf("Unable to open /dev/mem\n");
        exit(1);
    }
    
    gpio3_addr = (unsigned int*)mmap(0, GPIO3_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO3_START_ADDR);
    if (gpio3_addr == MAP_FAILED) {
        printf("Unable to map GPIO3\n");
        exit(1);
    }
    
    gpio_oe_addr = gpio3_addr + GPIO_OE;
    reg = *gpio_oe_addr;
    reg |= (1<<GP1_3); // 3 is an input
    reg &= ~(1<<GP1_4); // 4 is an output
    *gpio_oe_addr = reg;
    gpio_oe_addr = 0;
    reg = 0;
    
    gpio_datain = gpio3_addr + GPIO_DATAIN;
    gpio_clear_dataout = gpio3_addr + GPIO_CLEARDATAOUT;
    gpio_set_dataout = gpio3_addr + GPIO_SETDATAOUT;
    
    while (cont) {
        if ((*gpio_datain >> GP1_3)&0x1) {
            *gpio_clear_dataout = 1<<GP1_4;
        } else {
            *gpio_set_dataout = 1<<GP1_4;
        }
    }
    munmap((void*)gpio3_addr, GPIO3_SIZE);
    close(fd);
    return 0;
}
