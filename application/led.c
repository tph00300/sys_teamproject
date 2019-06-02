#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC_NUMBER	'H'
#define IOCTL_LED_ON		_IO(IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_LED_OFF		_IO(IOCTL_MAGIC_NUMBER, 1)

int led_fd;

void init_led ()
{
	led_fd = open("/dev/led_device", O_RDWR);

	if(led_fd < 0)
	{
		return -1
	}

}

void close_led()
{
	close(led_fd);
}

void on_led()
{
	ioctl(led_fd, IOCTL_LED_ON, 0);
}

void off_led()
{
	ioctl(led_fd, IOCTL_LED_OFF, 0);
}

