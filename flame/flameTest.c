#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CS_MCP3008 10
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000


int read_MCP3008(unsigned char adcChannel)
{
	unsigned char buff[3];
	int adcValue = 0;

	/*
	buff[0] = 0x06 | ((adcChannel & 0x07) >> 7);
	buff[1] = ((adcChannel & 0x07) << 6);
	buff[2] = 0x00;
	*/

	buff[0] = 1;
	buff[1] = (8+adcChannel)<<4;
	buff[2] = 0;

	digitalWrite(CS_MCP3008, 0);

	wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

	buff[1] = 0x0F & buff[1];
	//adcValue = (buff[1] << 8) | buff[2];
	adcValue = ((buff[1]&3) << 8) + buff[2];
	
	digitalWrite(CS_MCP3008, 1);
	
	return adcValue;
}

int main (void)
{
	int adcChannel = 0;
	int adcValue = 0;

	if(wiringPiSetup() == -1) return 1;
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1) return 1;

	pinMode(CS_MCP3008, OUTPUT);

	while(1)
	{
		//adcValue = read(adcChannel);
		adcValue = read_MCP3008(0)*3.3/1024;
		printf("adc0 Value = %u\n", adcValue);
		sleep(1);
	}

	return 0;
}

