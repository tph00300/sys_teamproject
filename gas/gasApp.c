#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC_NUMBER 'k'
#define IOCTL_GAS_READ  _IOR(IOCTL_MAGIC_NUMBER, 0, unsigned int)

#define GAS_IDLE    1
#define GAS_RELEASE 0

int main (void)
{
    int gas_fd = open("/dev/gas_device", O_RDONLY);

    int gas_status;
    while(1)
    {
        read(gas_fd, &gas_status, sizeof(int));
        printf("gas : %d\n", gas_status);

        //ioctl(gas_fd, IOCTL_GAS_READ, &gas_status);
        //printf("gas : %d\n", gas_status);

	sleep(1);
    }
}
