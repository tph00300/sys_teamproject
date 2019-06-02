#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "led.h"

#define IOCTL_MAGIC_NUMBER	'H'
#define IOCTL_LED_ON		_IO(IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_LED_OFF		_IO(IOCTL_MAGIC_NUMBER, 1)


int fd;

#endif
