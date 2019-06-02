/*
** buzzer.c 
** written by KSH, LTK in 19/06/02
** based on buzzer_test.c  
*/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<fcntl.h>

#include "buzzer.h"

#define NUM 100
#define WRITE _IOW(NUM, 1, int)

int buzzer_fd;

/*
** setup function to use buzzer's device driver
** in success case it will return 0 
*/
int init_buzzer()
{
	buzzer_fd = open("/dev/buzzer_device", O_RDWR); // for _IOW
	if (buzzer_fd < 0) {
		printf("buzzer init error\n");
		return 1;
	}

	return 0;
}

/*
** write '0' make buzzer on
** using ioctl function
*/
void on_buzzer()
{
	char buf[10] ="0";
	ioctl(buzzer_fd, WRITE, buf);

}

/*
** write '1' make buzzer on
** using ioctl function
*/
void off_buzzer()
{
	char buf[10] ="1";
	ioctl(buzzer_fd, WRITE, buf);
}
