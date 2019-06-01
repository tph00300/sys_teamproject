#include <pthread.h>
#include <unistd.h> // sleep
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int ncount;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int fd;

char buffer[100];

char TxPower[3];
char RSSI[4];

void* loop1(void *data)
{
	// initialize 'HM-10'		
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
				calc_dist(TxPower, RSSI);
			}
			else
			{
				printf("block\n");
			}
		}
		

		//putchar (serialGetchar (fd));
		//fflush(stdout);
	}

}

void calc_dist (char* TxPower, char* RSSI)
{
	char *end;
	int tx = strtol(TxPower, &end, 10);
	int rs = strtol(RSSI, &end, 10);

	printf("tx = %d, rs= %d\n", tx,rs);
}

int main()
{
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
	pthread_t p_thread[2];
	int status;
	int a = 1;

	ncount = 0;

	thr_id = pthread_create(&p_thread[0], NULL, loop1, (void *)&a);
	//sleep(1);
	thr_id = pthread_create(&p_thread[1], NULL, loop2, (void *)&a);
	

	pthread_join(p_thread[0], (void *) &status);
	pthread_join(p_thread[1], (void *) &status);

	status = pthread_mutex_destroy(&mutex);
	printf("code = %d", status);
	printf("programing is end");
	return 0;
}
