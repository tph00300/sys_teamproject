/*
** gas_app.c 
** wrtten by KimSeongHeon in 19/06/01
** based on gasTest.c  
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "gas.h"

#define IOCTL_MAGIC_NUMBER 'H'
#define IOCTL_GAS_READ  _IOR(IOCTL_MAGIC_NUMBER, 0, unsigned int)

#define GAS_IDLE    0
#define GAS_RELEASE 1

int gas_fd;


/*
** setup function to use 'gas_device' ReadOnly
** in NOT success case it will return 1
*/
int init_gas(void)
{
	gas_fd = open("/dev/gas_device", O_RDONLY);
	if(gas_fd < 0)
	{
		perror("failed open because ");
		return 1;
	}

	return 0;
}


/*
** read gas sensor's digital signal
** return 'gas_value'
** 1 = NO gas , 0 = YES gas
** use this function will be used in main program with while(1) to check gas signal consistently
*/
int read_gas(void)
{
	int gas_value;
	ioctl(gas_fd, IOCTL_GAS_READ, &gas_value);
    //printf("gas : %d\n", gas_status);

    return gas_value;
}


/*
** close 'gas_device' 
*/
void close_gas(void)
{
	close(gas_fd);
}
