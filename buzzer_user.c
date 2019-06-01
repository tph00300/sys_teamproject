#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<fcntl.h>

#define NUM 100
#define WRITE _IOW(NUM, 1, int)

int main(int argc, char **argv) {
	int fd = open("/dev/buzzer", O_RDWR);
	if (fd < 0) {
		printf("error\n");
		return 0;
	}
	printf("open\n");
	char buf[100];
	if (argc == 1) {
		scanf("%s", buf);
	}
	else {
		sscanf(argv[1], "%s", buf);
	}
	if (buf[0] == '2') {
		if (argc == 1 ){
			scanf("%s", buf + 2);
		}
		else {
			sscanf(argv[2], "%s", buf + 2);
		}
	}
	ioctl(fd, WRITE, buf);
	return 0;
}
