/*
** flame.c 
** wrtten by KSH in 19/05/31
** based on flame_test.c  
*/

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "flame.h"

#define CS_MCP3008 10 // wPi 10
#define SPI_CHANNEL 0 
#define SPI_SPEED 1000000

/*
** setup function to use wiringPi and wiringPISPI
** in success case it will return 0 
*/
int init_MCP3008 (void)
{
	if(wiringPiSetup() == -1) return 1;
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) return 1;

	pinMode(CS_MCP3008, OUTPUT);

	return 0;
}


/*
** read flame sensor's analog signal using 'read_MCP3008' function
** return 'flame_adcValue' which is calculated digital signal
** 1 = flame NOT exist , 0 = flame exist
** use this function will be used in main program with while(1) to check flame signal consistently
*/
int read_flame() 
{	
	
	int adcValue = 0;
	int adcChannel = 0;

    adcValue = read_MCP3008((unsigned char)adcChannel)*3.3/1024;
	// printf("adc0 Value = %u\n", adcValue);
    sleep(1);

	return adcValue;


	/*
	unsigned char buff[3];

	buff[0] = 1;
	buff[1] = (8+(unsigned char)adcChannel)<<4;
	buff[2] = 0;

	digitalWrite(CS_MCP3008, 0);
	wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

	adcValue = ((buff[1]&3 << 8) + buff[2]);
	digitalWrite(CS_MCP3008,1);

	return adcValue*3.3/1024;
	*/
}


/*
** convert flame module's analog signal to digital using MCP3008 module
** return 'adcValue' digital siganl conveted by MCP3008 module
** this function will be used in 'read_flame' function
*/
int read_MCP3008(unsigned char adcChannel)
{
	unsigned char buff[3];
	int adcValue = 0;

	buff[0] = 1;
	buff[1] = (8+adcChannel)<<4;
	buff[2] = 0;

	digitalWrite(CS_MCP3008, 0);

	wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

	//buff[1] = 0x0F & buff[1];
	adcValue = ((buff[1]&3) << 8) + buff[2];
	
	digitalWrite(CS_MCP3008, 1);
	
	return adcValue;
}
