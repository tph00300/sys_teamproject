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

void* loop1(void *data)
{
	/*
	int i;
	pthread_mutex_lock(&mutex);
	for (i =0; i< 10; i++)
	{
		//pthread_mutex_lock(&mutex);
		printf("loop1 : %d\n", ncount);
		ncount ++;
		//if(i ==10){pthread_mutex_unlock(&mutex);}
		//sleep(1);
	}
	pthread_mutex_unlock(&mutex);
	*/
	int i;
	for(i =0; i<3; i++){
	pthread_mutex_lock(&mutex);
	char c[15] = "AT+DISI?\0";
	serialPuts(fd, c);
	serialPuts(fd, "\r\n");
	pthread_mutex_unlock(&mutex);
	sleep(5);
	}
}
char buffer[72];

void* loop2(void *data)
{
	/*
	int i;
	for (i =0; i< 10; i++)
	{
		//pthread_mutex_lock(&mutex);
		printf("loop2 : %d\n", ncount);
		ncount ++;
		//pthread_mutex_unlock(&mutex);
		//sleep(2);
	}
	*/
	//int i = 0;

	while(1)
	{
		//putchar(serialGetchar(fd));
		//fflush(stdout);
		int count = 0;
		if(putchar(serialGetchar(fd)) == 'O')
		{
			char tmp;
			while(count<7)
			{
				tmp = serialGetchar (fd);
				count++;
			}
			count = 0;
			if(tmp == 'C')
			{
				for(int i = 0; i < 72; i++)
				{
					buffer[i] = serialGetchar(fd);
				}

				printf("%s",buffer);
			}
		}
	}

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
