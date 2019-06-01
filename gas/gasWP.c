#include <stdio.h>
#include <wiringPi.h>

#define D0 22

int main()
{
	wiringPiSetupGpio();

	pinMode(D0, OUTPUT);

	while(1) 
	{
		if(digitalRead(D0) == LOW)
			puts("Alcohol Level is low!");
		else
			puts("Alcohol Level is high!");

		delay(1000);
	}

	return 0;
}

