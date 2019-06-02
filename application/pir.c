#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<string.h>
#define MY_S 100
#define READ _IOR( MY_S, 0, int )
#define WRITE _IOW( MY_S, 1, int )

int pir_fd;

int init_pir(){
	pir_fd = open("/dev/pir_device", O_RDWR);
	if (pir_fd < 0) {
		printf("file not open\n");
		return 1;
	}
	return 0;
}
int read_pir(){

	char buf[100]={0,};
	ioctl(pir_fd,READ,buf);
	return atoi(buf);
}
/*	int n=0,t=0,f=0;
	while(1){
		sleep(1);
		ioctl(fd, READ, buf);
		int flag=atoi(buf);
		if(flag==0)
			t++;
		else if(flag==1)
			f++;
		n++;
		if(n>=30)
		{
			printf("%d%%\n",f*3);
			n=0;
			t=0;
			f=0;
		}
	}*/
