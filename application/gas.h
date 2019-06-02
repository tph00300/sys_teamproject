#ifndef _GAS_H_
#define _GAS_H_

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "gas.h"

#define IOCTL_MAGIC_NUMBER 'k'
#define IOCTL_GAS_READ  _IOR(IOCTL_MAGIC_NUMBER, 0, unsigned int)

#define GAS_IDLE    0
#define GAS_RELEASE 1

int gas_fd;

int init_gas(void);
int read_gas(void);
int close_gas(void);


#endif