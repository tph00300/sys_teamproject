#ifndef _PIR_H_
#define _PIR_H_

#include<stdio.h> 
#include<unistd.h> 
#include<fcntl.h> 
#include<sys/ioctl.h> 
#include<stdlib.h> 
#include<string.h> 

#include "pir.h"


#define MY_S 100 
#define READ _IOR( MY_S, 0, int ) 
#define WRITE _IOW( MY_S, 1, int )

int pir_fd;
int init_pir();
int read_pir();
#endif
