#include <pthread.h>
#include <unistd.h> // sleep
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h> // strtol

#include <wiringPi.h>
#include <wiringSerial.h>

#include <math.h>

#include "buzzer.h"
#include "flame.h"
#include "gas.h"
#include "led.h"
#include "pir.h"

int ncount;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int fd;
char buffer[100];
char TxPower[3];
char RSSI[4];

void* loop1(void *data)
{
	// setup iBeacon		
	printf("AT+RENEW\n"); // Factoy reset
	serialPuts(fd, "AT+RENEW\0");
	serialPuts(fd, "\r\n");
	sleep(2);

	printf("AT+RESET\n"); // Reboot
	serialPuts(fd, "AT+RESET\0");
	serialPuts(fd, "\r\n");
	sleep(2);

	printf("AT\n"); // waiting OK mesg
	serialPuts(fd, "AT\0");
	serialPuts(fd, "\r\n");
	sleep(2);

	printf("AT+IBEA1\n"); // iBeacon mode
	serialPuts(fd, "AT+IBEA1\0");
	serialPuts(fd, "\r\n");
	sleep(2);

	printf("AT+MODE2\n"); // Set device as 'remote control code' (it can use AT command after pairing)
	serialPuts(fd, "AT+MODE2\0");
	serialPuts(fd, "\r\n");
	sleep(2);

	// set and check ROLE1 and IMME1 to use AT+DISI? command
	printf("AT+ROLE1\n"); // Set Device as 'Central'
	serialPuts(fd, "AT+ROLE1\0");
	serialPuts(fd, "\r\n");
	sleep(2);

	printf("AT+IMME1\n"); // Set operation type as 'Not immedately' 
	serialPuts(fd, "AT+IMME1\0");
	serialPuts(fd, "\r\n");
	sleep(2);
	
	printf("AT+IMME?\n");
	serialPuts(fd, "AT+IMME?\0");
	serialPuts(fd, "\r\n");
	sleep(2);
	
	printf("AT+ROLE?\n");
	serialPuts(fd, "AT+ROLE?\0");
	serialPuts(fd, "\r\n");
	sleep(2);

	for(int i =0; i<3; i++)
	{
		printf("FOR LOOP\n");
		pthread_mutex_lock(&mutex);
		serialPuts(fd, "AT+DISI?\0");
		serialPuts(fd, "\r\n");
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
}

double calc_dist (char* TxPower, char* RSSI)
{
	char *end;
	int tx = strtol(TxPower, &end, 16);
	tx = (int)(tx|0xffffff00u);
	int rs = strtol(RSSI, &end, 10);
	rs *= -1;

	//int dist = 10^((tx-rs)/(10*2));
	double dist= pow(10,((tx-rs)/(20)));
	printf("tx = %d, rs= %d dist= %.8f\n", tx,rs,dist);

	return dist;
}

void* loop2(void *data)
{
	while(1)
	{
		if(serialGetchar(fd) == 'O')
		{
			char tmp[7];
			for(int i = 0; (i < 7)&&(serialDataAvail(fd) != -1); i++)
			{
				tmp[i] = serialGetchar(fd);
			}

			//if((tmp[6] != 'E')&&(tmp[6] != 'S'))
			if(tmp[6] == ':')
			{
				for(int j = 0;serialDataAvail(fd) != -1; j++)
				{
					buffer[j] = serialGetchar(fd);			
					if(buffer[j] == '\n') break;
				}
				printf("line\n");
				printf("%s",buffer);
				
				TxPower[0] = buffer[50]; 
				TxPower[1] = buffer[51];

				RSSI[0] = buffer[67];
				RSSI[1] = buffer[68];
				RSSI[2] = buffer[69];

				printf("TxPower: %s, RSSI: %s\n",TxPower, RSSI);
			}
			else
			{
				printf("block\n");
			}
		}
/*
		putchar (serialGetchar (fd));
		fflush(stdout);
*/
	}

}

void* loop3(void *data)
{
	printf("check pir\n");
	printf("init pir\n");
	int check=init_pir();
	if(check!=0)
	{
		printf("end error\n");
		return 0;
	}
	int n=0,t=0,f=0;
	
	while(1){
		sleep(1);
		int flag=read_pir();
		if(flag==0)
			t++;
		else if(flag==1)
			f++;
		n++;
		if(n>=30)
		{
			printf("%d%%\n",f*3);
			n=0;
			t=0;
			f=0;
		}
	}
}

int main()
{
	int result = 1;

	//check flame
	printf("check flame\n");
	//printf("init_MCP3008\n");
	int check = init_MCP3008();

	if(check != 0)
	{
		printf("end program(ERR)\n");
		return 0;
	}

	while(result != 0)
	{
		result = read_flame();
		printf("flame result : %d\n", result);
	}

	/*
	//check gas
	printf("check gas\n");
	printf("init_gas\n");
	check = init_gas();

	if(check != 0)
	{
		printf("end program(ERR)\n");
		return 0;
	}

	while(result == 0)
	{
		int result = read_gas();
		printf("gas result : %d\n", result);
	}

	close_gas();
	*/

	if(result == 0)
	{
		//on buzzer
		printf("on_buzzer\n");
		check = init_buzzer();
		if(check != 0)
		{
			printf("end program(ERR)\n");
			return 0;
		}
		on_buzzer();

		//on led
		printf("on_led");
		init_led();
		on_led();


		//init iBeacon device
		if((fd = serialOpen ("/dev/serial0",9600)) < 0)
		{
			fprintf(stderr,"Unable to open serial device: %s\n", strerror (errno));
			return 1;
		}

		if (wiringPiSetup () == -1)
		{
			fprintf(stdout, "Unable to start wiringPi: %s\n", strerror (errno));
			return 1;
		}

		int thr_id;
		pthread_t p_thread[3];
		int status;
		int a = 1;

		thr_id = pthread_create(&p_thread[0], NULL, loop1, (void *)&a);
		thr_id = pthread_create(&p_thread[1], NULL, loop2, (void *)&a);
		thr_id = pthread_create(&p_thread[2], NULL, loop3, (void *)&a);
	
		pthread_join(p_thread[0], (void *) &status);
		pthread_join(p_thread[1], (void *) &status);

		status = pthread_mutex_destroy(&mutex);
		printf("code = %d", status);
		printf("programing is end");
	}
	

	return 0;
}
