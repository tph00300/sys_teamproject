#include <stdio.h>
#include <string.h>
#include <errno.h>
 
#include <wiringPi.h>
#include <wiringSerial.h>
 
int main ()
{
	int fd ;
  
	int count ;
  
	unsigned int nextTime ;

	if ((fd = serialOpen ("/dev/serial0", 9600)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	if (wiringPiSetup () == -1)
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
       	}

	//int i = 0;
	while(1)
	{
		char s[10];
	       	scanf("%s",s);
		serialPuts (fd,s);
		serialPuts (fd,"\r\n");

		//printf("input : %s\n",s);
		
		//char c[15] = "AT+DISI?\0"; // \0 없으면 입력값 전송 안됨
	       	//scanf("%s",c);
		//serialPuts (fd,c);
		//serialPuts (fd,"\r\n"); // 없으면 개행 없는 응답

		//printf("input : %s\n",c);

		
		while (serialDataAvail (fd))
		//while(1)
		{
			//printf("\\");
			putchar (serialGetchar (fd));
			fflush(stdout);
   
	       	}
		

	}
	
	return 0 ;
}
