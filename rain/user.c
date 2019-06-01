#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<string.h>
#define MY_S 100
#define READ _IOR( MY_S, 0, int )
#define WRITE _IOW( MY_S, 1, int )

int main(int argc, char **argv) {
	int fd = open("/dev/ltk", O_RDWR);
	if (fd < 0) {
		printf("file not open\n");
		return 0;
	}

	
	char buf[100]={0,};
	int q[11]={0,};
	int w[2]={0,},n=0,m=0;
	while(1){
		m++;
		sleep(1);
		ioctl(fd, READ, buf);
		int flag=atoi(buf);
		w[flag]++;
		if(n==10)
			n=0;
		if(m>=10)
			w[q[n]]--;
		q[n]=flag;
		n++;
		printf("%d\n",w[1]*10);
	}
	return 0;
}
