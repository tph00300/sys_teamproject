#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h> // 파일시스템 사용
#include <linux/uaccess.h> // copy_to_user 이용
#include <linux/slab.h> // kmaloc() 사용
#include <linux/gpio.h>

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
	printk(KERN_INFO "read the output of device (value : %d) from kernel\n", value);
	int check = copy_to_user(buf, &value, sizeof(int));

	if(check != 0)
	{
		printk(KERN_INFO "copy_to_user ERR\n");
	}
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

static struct  file_operations fops =
{
	.read = gas_device_read,
	//.write = buzzer_device_write,
	.open = gas_device_open,
	.release = gas_device_release
};

int __init gas_device_init(void)
{
	printk(KERN_INFO "gas_device init module\n");

	register_chrdev(240,"gas_device", &fops);

	gpio_request(GPIO23, "GPIO23");
	gpio_direction_input(GPIO23);

	printk(KERN_INFO "gas_device init module complete\n");

	return 0;
}

void __exit gas_device_exit(void)
{
	gpio_free(GPIO23);
	unregister_chrdev(240, "gas_device");
	printk(KERN_ALERT "gas_device cleanup successful\n");
}

module_init(gas_device_init);
module_exit(gas_device_exit);
MODULE_LICENSE("GPL");
