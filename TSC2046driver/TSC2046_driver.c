 /* @file TSC2046_driver.c
 * @author Alexander Hirschfeld
 * @date   31 Oct, 2017
 * @brief  A kernel module for controlling the TSC2046 Touch Screen IC. 
 * This uses an 8bit SPI command and receives an 8 or 12bit response.
 * @version v0.1
 * @liscense GPL
 */
 
 /*
 PENIRQ goes low when touch is registered, using this as interrupt on a pin to know when to start reading.
 
 Command bit string:
 
 7  6   5   4   3   2   1   0
 1  A2  A1  A0  M   SER PD1 PD0
 
 7: Start bit, always 1
 A2-0: Mux Select 
 
 M: Mode: 12 bit response when low, 8 bit when high, speed of conversion, and accuracy of touch screen
 
 SER/`DFR Single refrence or diffrential refrence, When high, it is single refrence, low id diffrential, see datasheet
 http://www.ti.com/lit/ds/symlink/tsc2046.pdf
 
 PD1-0 Power down mode, what to do after successful read.
 
 A2-0
 2 1 0: result
 0 0 1: Measure y
 0 1 1: Measure z1
 1 0 0: Measure z2
 1 0 1: Measure x
 
 PD1-0
 1 0: result
 0 0: penirq enabled, power down between conversions
 0 1: penirq disabled, refrence is off, adc is on
 1 0: penirq enabled, refrence is on, adc is off
 1 1: penirq diabled, refrence is on, adc is on.
 
 */
 
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>
#include <linux/stat.h>
#include <linux/sysfs.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hirschfeld");
MODULE_DESCRIPTION("SPI driver for TSC2046");
MODULE_VERSION("0.1");

#define DRIVER_NAME "TSC2046"

#define GPIO_TO_PIN(bank, pin) ((32 * bank) + pin);
#define DEVICE_NAME "TSC2046"
#define SPI_BUS_SPEED 2000000 //2MHz

static struct spi_device *spi_dev;

static struct workqueue_struct* tsc2046_workqueue;

static short penirq;
static short spiport;
static short spics;

static unsigned int irqNumber; 

struct screenVals {
    int x;
    int y;
    int z1;
    int z2;
}

struct diffVals {
    unsigned int time_down;
    int diff_x;
    int diff_y;
    int diff_z1;
    int diff_z2;
}

// Module Params
module_param(spics, short, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(spics, "Chip Select");
module_param(spiport, short, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(spiport, "SPI port");
module_param(penirq, short, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(penirq, "Pen Interrupt Port");

// Interrupts
static irq_handler_t penirq_interrupt_handler(unsigned int irq, void* dev_id, struct pt_regs *reg);

// Kobject and sysfs
static struct kset *TSC2046_kset;

struct TSC2046_obj {
    struct kobject kobj;
    struct screenVals vals;
    struct diffVals diffs;
    bool reading;
    struct work_struct get_value;
};
#define to_TSC2046_obj(x) container_of(x, struct TSC2046_obj, kobj)

struct TSC2046_attr {
    struct attribute attr;
    ssize_t (*show)(struct TSC2046_obj* obj, struct TSC2046_attr* attr, char* buf);
    ssize_t (*store)(struct TSC2046_obj* obj, struct TSC2046_attr* attr, const char* buf, size_t count);
};
#define to_TSC2046_attr(x) container_of(x, struct TSC2046_attr, attr);

static struct

static struct TSC2046_obj* init_TSC2046_obj(const char* name) {
    struct TSC2046_obj* obj;
    int retval;
    
    obj = kzalloc(sizeof(*obj), GPF_KERNEL);
    if !(obj) {
        return NULL;
    }
    
    obj->kobj.kset = TSC2046_kset;
    retval = kobject_init_and_add(&obj->kobj, )
}

static ssize_t TSC2046_vals_show(struct TSC2046_obj* obj, struct TSC2046_attr* attr, char* buf) {
    return 0;
}

static size_t TSC2046_diffs_show(struct TSC2046_obj* obj, struct TSC2046_attr* attr, char* buf) {
    return 0;
}

static ssize_t TSC2046_batt_show(struct TSC2046_obj* obj, struct TSC2046_attr* attr, char* buf) {
    return 0;
}

static ssize_t TSC2046_temp_show(struct TSC2046_obj* obj, struct TSC2046_attr* attr, char* buf) {
    return 0;
}

static ssize_t TSC2046_PD_show(struct TSC2046_obj* obj, struct TSC2046_attr* attr, char* buf) {
    return 0;
}

static ssize_t TSC2046_PD_store(struct TSC2046_obj* obj, struct TSC2046_attr* attr, const char* buf, size_t count) {
    return 0;
}

static ssize_t TSC2046_oneshot_show(struct TSC2046_obj* obj, struct TSC2046_attr* attr, char* buf) {
    return 0;
}

static ssize_t TSC2046_oneshot_store(struct TSC2046_obj* obj, struct TSC2046_attr* attr, const char* buf, size_t count) {
    return 0;
}

static ssize_t TSC2046_null_store(struct TSC2046_obj* obj, struct TSC2046_attr* attr, const char* buf, size_t count) {
    return 0;
}

// Initializations
static int __init initialization(void){
    int status;
    
    struct spi_master* master;
    struct device* temp;
    char buff[20];
    
    printk(KERN_INFO "TSC2046: Initializing\n");
    
    printk(KERN_INFO "TSC2046: Params :: spics: %hd :: spiport:%hd :: penirq: %hd\n", spics, spiport, penirq);
    
    TSC2046_kset = kset_create_and_add(DRIVER_NAME, NULL, firmware_kobj);
    if (!TSC2046_kset) {
        kset_unregister(TSC2046_kset);
        printk(KERN_INFO "TSC2046: Unable to register kset in firmware");
        return -EINVAL;
    }
    
    if (spics == 0 && spiport == 0 && penirq == 0) {
        printk(KERN_INFO "TSC2046: Invalid params, please values for spics, spiport, and penirq\n");
        return -EINVAL;
    }
    printk(KERN_INFO "TS2046: Passed params\n");
    if (penirq == 0) {
        printk(KERN_INFO "TSC2046: Please pass a valid pin for penirq\n");
        return -EINVAL;
    }
    printk(KERN_INFO "TS2046:passed penirq check\n");
    master = spi_busnum_to_master(spiport);
    if (!master) {
        printk(KERN_INFO "TS2046: Init Failed, Bad Master port, %dh", spiport);
        return -EINVAL;
    }
    printk(KERN_INFO "TS2046:pass busnum to master\n");
    spi_dev = spi_alloc_device(master);
    if (!spi_dev) {
        put_device(&master->dev);
        printk(KERN_INFO "TSC2046: Init Failed, Bad Device\n");
        return -EINVAL;
    }
    printk(KERN_INFO "TS2046:passed spi_alloc\n");
    spi_dev->chip_select = spics;
    snprintf(buff, sizeof(buff), "%s.%u", dev_name(&spi_dev->master->dev), spi_dev->chip_select);
    
    // Attempt to find the device, and if found hijack it.
    temp = bus_find_device_by_name(spi_dev->dev.bus, NULL, buff);
    if (temp) {
        spi_unregister_device(to_spi_device(temp));
        spi_dev_put(to_spi_device(temp));
    }
    printk(KERN_INFO "TS2046:passed device busy\n");
    spi_dev->max_speed_hz = SPI_BUS_SPEED;
    spi_dev->mode = SPI_MODE_0;
    spi_dev->bits_per_word = 8;
    spi_dev->irq = -1;
    spi_dev->controller_data = NULL;
    spi_dev->controller_state = NULL;
    strcpy(spi_dev->modalias, DRIVER_NAME);
    
    status = spi_add_device(spi_dev);
    
    if (status < 0) {
        spi_dev_put(spi_dev);
        put_device(&master->dev);
        printk(KERN_INFO "TS2046: Init Failed: Failed to add device\n");
        return status;
    }
    printk(KERN_INFO "TS2046:pass spi add\n");
    
    gpio_request(penirq, "sysfs");
    gpio_direction_input(penirq);
    gpio_set_debounce(penirq, 200);
    gpio_export(penirq, false);
    
    irqNumber = gpio_to_irq(penirq);
    
    status = request_irq(irqNumber,             // The interrupt number requested
                        (irq_handler_t) penirq_interrupt_handler, // The pointer to the handler function below
                        IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,   // Interrupt on rising edge (button press, not release)
                        "penirq_interrupt_handler",    // Used in /proc/interrupts to identify the owner
                        NULL);
    
    return status;
}

static void __exit exiting(void) {
    spi_dev_put(spi_dev);
    free_irq(irqNumber, NULL);
    gpio_free(penirq);
    printk(KERN_INFO "TSC2046: Goodbye");
    
}

static irq_handler_t penirq_interrupt_handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
    printk(KERN_INFO "TSC2046: Trigger for IRQ, state: %d\n", gpio_get_value(penirq));
    return (irq_handler_t) IRQ_HANDLED;
}

module_init(initialization);
module_exit(exiting);