#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_MAGIC_NUMBER	'H'
#define IOCTL_LED_ON		_IO(IOCTL_MAGIC_NUMBER, 0)
#define IOCTL_LED_OFF		_IO(IOCTL_MAGIC_NUMBER, 1)

void led_one()
{
   int fd = open("/dev/led_device", O_RDWR);
   int mass = 0;
   
   while(1){
	   
	   printf("LED input\n");
	   scanf("%d", &mass);
	   
	   if (mass == 1) {
			printf("Turnning on LED\n");
			ioctl(fd, IOCTL_LED_ON, 0);
	   }
	   else if (mass == 0) {
		    printf("Turnning off LED\n");
			ioctl(fd, IOCTL_LED_OFF, 0);
	   }
	   else {
		       printf("usage : led_app [0|1] \n");
               break;			   
	   }
	   
   }

   close(fd);
}

int main () {
	
	led_one();
	
	return 0;
	
}


  


 



