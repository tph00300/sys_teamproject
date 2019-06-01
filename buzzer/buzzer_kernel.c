#include<linux/gpio.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/ktime.h>
#include<linux/hrtimer.h>
#include<linux/types.h>
#include<linux/fcntl.h>

#define MY_T_SENSOR 100
#define MY_T_SENSOR_READ _IOW( MY_T_SENSOR, 1, int )

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("guithin(JinwonJung)");

struct cdev my_cdev;
dev_t devno;
int major;
int pin = 7;

ktime_t ktime_h, ktime_l;
static struct hrtimer hr_timer;
int state;
int run;
char *msg;

enum hrtimer_restart my_hrtimer_callback(struct hrtimer *timer) {
    gpio_set_value(pin, state ^= 1);
    if (run) hrtimer_start(&hr_timer, state ? ktime_h : ktime_l, HRTIMER_MODE_REL);
    return HRTIMER_NORESTART;
}

void timer_init(void) {
    ktime_h = ktime_set(0, 1000 * 1000);
    ktime_l = ktime_set(0, 3000 * 1000);
    hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hr_timer.function = &my_hrtimer_callback;
}

void sound_on(void) {
    if (run) return;
    run = 1;
    hrtimer_start(&hr_timer, 1000, HRTIMER_MODE_REL);
}

void sound_off(void) {
    run = 0;
}

void set_sound_hz(int x) {
    ktime_h = ktime_set(0, x * 1000);
    ktime_l = ktime_set(0, 3 * x * 1000);
}

int tsensor_dev_open(struct inode *pinode, struct file *pfile){
    printk("Open tsensor drv\n");
    return 0;
}

int tsensor_dev_close(struct inode *pinode, struct file *pfile){
    printk("Close tsensor drv\n");
    return 0;
}

/*
 * msg
 * 0 : sound on
 * 1 : sound off
 * 2 x : set delay (cycle: x*4 us)
*/
long tsensor_dev_write(struct file *pfile, unsigned int command, unsigned long arg){
    if(command == MY_T_SENSOR_READ){
        memset(msg, 0, 32);
        if (copy_from_user((void *)msg, (const void *)arg, (ssize_t)strlen((char *)arg))) {
            printk("read error\n");
        }
        if (msg[0] == '0') {
            sound_on();
        }
        else if (msg[0] == '1') {
            sound_off();
        }
        else if (msg[0] == '2') {
            int k = 0;
            char *i = msg + 1;
            while((*i)) {
                k *= 10;
                k += (*i) - '0';
            }
            set_sound_hz(k);
        }
        else {
            printk("cmd error\n");
        }
    }
    return 0;
}

struct file_operations fop = {
    .owner = THIS_MODULE,
    .open = tsensor_dev_open,
    .unlocked_ioctl = tsensor_dev_write,
    .release = tsensor_dev_close,
};

int __init tsensor_init(void){
    printk("INIT tsensor moter\n");

    alloc_chrdev_region(&devno, 0, 2, "tsensor_dev");
    major = MAJOR(devno);
    
    cdev_init(&my_cdev, &fop);
    my_cdev.owner = THIS_MODULE;

    printk("MAJOR: %d\n", MAJOR(devno));

    if(cdev_add(&my_cdev, devno, 1) < 0){
        printk("Device add err\n");
        return -1;
    }

    if(gpio_request(pin, "TSENSOR_PIN") < 0){
        printk("gpio_request fail\n");
    }
    msg = (char *)kmalloc(32, GFP_KERNEL);

    gpio_direction_output(pin, 0);
    timer_init();
    return 0;
}

void __exit tsensor_exit(void){
    printk("EXIT tsensor module\n");
    if (msg) kfree(msg);
    gpio_free(pin);
    cdev_del(&my_cdev);
}

module_init(tsensor_init);
module_exit(tsensor_exit);