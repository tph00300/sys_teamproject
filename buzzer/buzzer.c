#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<fcntl.h>

#define NUM 100
#define WRITE _IOW(NUM, 1, int)

int buzzer_fd;

int init_buzzer()
{
	buzzer_fd = open("/dev/buzzer_device", O_RDWR);
	if (buzzer_fd < 0) {
		printf("buzzer init error\n");
		return 1;
	}

	return 0;
}

void on_buzzer()
{
	ioctl(buzzer_fd, WRITE, '0');

}

void off_buzzer()
{
	ioctl(buzzer_fd, WRITE, '1');
}