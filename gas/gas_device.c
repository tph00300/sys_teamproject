#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h> // 파일시스템 사용
#include <linux/uaccess.h> // copy_to_user 이용
#include <linux/slab.h> // kmaloc() 사용

#define GPIO23 23

#define IOCTL_MAGIC_NUMBER 'k'
#define IOCTL_GAS_READ _IOR(IOCTL_MAGIC_NUMBER, 0, int)

int gas_device_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "gas_device open function called\n");
	return 0;
}

int gas_device_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "gas_device release function called\n");
	return 0;
}

/*
ssize_t buzzer_device_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int data = 0;
	copy_from_user(&data, buf, count);
	bcm2835_pwm_set_data(PWM_CHANNEL, data);
	printk(KERN_INFO "write the data %d to kernel\n", data);

	return count;
}
*/

ssize_t gas_device_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int value = gpio_get_value(GPIO23);
	printk(KERN_INFO "read the output of device %d from kernel\n", value);
	copy_to_user(buf, &value, sizeof(int));
	return count;
}

long gas_ioctl(struct file * filp, unsigned int cmd, unsigned long arg)
{
	int param_size;
	unsigned int value;

	/*
	if(_IOC_TYPE(cmd) != IOCTL_MAGIC) return -EINVAL;
	If(_IOC_NR(cmd) >= IOCTL_MAXNR) return -EINVAL;
	if( size )
	{
       err = -EFAULT;
       if( _IOC_DIR( cmd ) & _IOC_READ  )
                err = !access_ok( VERIFY_WRITE, (void __user *) arg, size );
       else if( _IOC_DIR( cmd ) & _IOC_WRITE )
                err = !access_ok( VERIFY_READ , (void __user *) arg, size );
       if( err ) return err;
    }
	*/

	switch(cmd)
	{
		case IOCTL_GAS_READ:
			value = gpio_get_value(GPIO23);
			copy_to_user((int*)arg, &value, sizeof(int));
			printk(KERN_INFO "read the output of device %d from kernel\n", value);
			break;
	}

	return 0;
}

static struct  file_operations sys_fops =
{
	.read = buzzer_device_read,
	//.write = buzzer_device_write,
	.open = buzzer_device_open,
	.release = buzzer_device_release
};

int __init gas_device_init(void)
{
	if(register_chrdev(240, "gas_device", &sys_fops) < 0)
		printk(KERN_ALERT "gas_device init failed\n");
	else
	{
		printk(KERN_ALERT "gas_device init successful\n");
		if(!bcm2835_init())
		{
			printk(KERN_ALERT "gas_device init successful\n");
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
	unregister_chrdev(240, "gas_device");
	printk(KERN_ALERT "gas_device cleanup successful\n");
}

module_init(gas_device_init);
module_exit(gas_device_exit);
MODULE_LICENSE("GPL");
