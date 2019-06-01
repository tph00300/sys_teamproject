#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>

#define MY_SERVO_NUM 100
#define MY_SERVO_WRITE _IOW( MY_SERVO_NUM, 1, int )

int main(int argc, char **argv){
    int fd = open("/dev/servo_dev", O_RDWR);
    if(fd < 0){
        perror("Fail to open\n");
        return errno;
    }
    printf("open drv\n");
    char buf[100];
    if(argc == 1){
        scanf("%s", buf);
    }
    else{
        sscanf(argv[1], "%s", buf);
    }
    ioctl(fd, MY_SERVO_WRITE, buf);
    return 0;
}