#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include <bcm2835.h>

#define PIN_PWM RPI_GPIO_P1_12
#define PWM_CHANNEL 0
#define RANGE 1024

static int sum = 0;

int buzzer_device_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "buzzer_device open function called\n");
	return 0;
}

int buzzer_device_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "buzzer_device release function called\n");
	return 0;
}

ssize_t buzzer_device_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int data = 0;
	copy_from_user(&data, buf, count);
	bcm2835_pwm_set_data(PWM_CHANNEL, data);
	printk(KERN_INFO "write the data %d to kernel\n", data);

	return count;
}

ssize_t buzzer_device_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_INFO "read the number %d from kernel\n", sum);
	copy_to_user(buf, &sum, 4);
	return count;
}

static struct  file_operations sys_fops =
{
	.read = buzzer_device_read,
	.write = buzzer_device_write,
	.open = buzzer_device_open,
	.release = buzzer_device_release
};

int __init buzzer_device_init(void)
{
	if(register_chrdev(240, "buzzer_device", &sys_fops) < 0)
		printk(KERN_ALERT "driver init failed\n");
	else
	{
		printk(KERN_ALERT "driver init successful\n");
		if(!bcm2835_init())
		{
			printk(KERN_ALERT "bcm2835 init successful\n");
			return 1;
		}

		bcm2835_gpio_fsel(PIN_PWM,BCM2835_GPIO_FSEL_ALT5);
		bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
		bcm2835_pwm_set_mode(PWM_CHANNEL,1,1);
		bcm2835_pwm_set_range(0,RANGE);
	}
	return 0;
}

void __exit buzzer_device_exit(void)
{
	unregister_chrdev(240, "buzzer_device");
	printk(KERN_ALERT "driver cleanup successful\n");
}

module_init(buzzer_device_init);
module_exit(buzzer_device_exit);
MODULE_LICENSE("GPL");



