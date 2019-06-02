#ifndef _FLAME_H_
#define _FLAME_H_

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "flame.h"

#define CS_MCP3008 10 
#define SPI_CHANNEL 0 
#define SPI_SPEED 1000000

int init_MCP3008 (void);
int read_flame();
int read_MCP3008(unsigned char adcChannel);

#endif
