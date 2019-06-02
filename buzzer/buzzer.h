#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<fcntl.h>

#include "buzzer_app.h"

#define NUM 100
#define WRITE _IOW(NUM, 1, int)

int buzzer_fd;

int init_buzzer();
void on_buzzer();
void off_buzzer();