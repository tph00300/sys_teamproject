#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>

#include <unistd.h> // sleep
#include <errno.h>

#include <pthread.h>

#include "buzzer.h"
#include "flame.h"
#include "gas.h"
#include "led.h"
#include "pir.h"

#define MAXLINE 1024

char line[MAXLINE], sendline[MAXLINE], recvline[MAXLINE+1];
int n, size, comp, addr_size;
pid_t fork_ret;

static int s;

char *escapechar = "exit"; // 종료 문자열

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void z_handler()
{
  int state;
  waitpid(-1, &state, WNOHANG);
  exit(0);

  return ;
}

void start_flame()
{
	printf("check flame\n");
	printf("init_MCP3008\n");
	int check = init_MCP3008();

	if(check != 0)
	{
		printf("end program(flame)\n");
	}
}

void start_gas()
{
	printf("check gas\n");
	printf("init_gas\n");
	int check = init_gas();

	if(check != 0)
	{
		printf("end program(gas)\n");
	}
}

void start_buzzer()
{
	printf("check buzzer\n");
	printf("init_buzzer\n");
	int check = init_buzzer();

	if(check != 0)
	{
		printf("end program(buzzer)\n");
	}

	on_buzzer();
}

void start_led()
{
	printf("check led\n");
	printf("init_led");
	init_led();

	on_led();
}

char string[8] = "0 1\n"; // NO FIRE
int check[3] = {0, 0, 0}; // loop1 X, loop3 X, loop4 X

void* loop1(void *data) // flame, gas
{
	start_flame();
	start_gas();
	
	int flame_result = 1;
	int gas_result = 0;

	while(1)
	{
		flame_result = read_flame();
		//printf("flame result : %d\n", flame_result);
		
		//gas_result = read_gas();
		printf("gas result : %d\n", gas_result);

		if(flame_result == 0 || gas_result == 1)
		{
			string[2] = '0';
			check[0] = 1; // loop 1 want to write
			
			printf("string : %s\n", string);

			start_buzzer();
			start_led();

			break;
		}
	}

	printf("ENDWHILE\n");
	/*

		pthread_mutex_lock(&mutex);
		printf("LOOP 1 is free!");
		printf("string : %s", string);
		sendline[0]='0';
		sendline[1]=' ';
		sendline[2]='' 
		check = 1;
		printf("string : %s", string);
		pthread_mutex_unlock(&mutex);
		sleep(2);
	*/
}

void* loop2(void *data) // socket
{
	fork_ret = fork();
	if(fork_ret > 0)
	{
		// 부모 프로세스는 키보드 입력을 서버로 송신
		//while(fgets(sendline, MAXLINE, stdin) != NULL)
		while((check[0] == 1) || (check[1] == 1) || (check[2] == 1))
		{
			//size = strlen(sendline);
			size = strlen(string);
			//if(write(s, sendline, strlen(sendline)) != size)
			if(write(s, string, strlen(string)) != size)
			{
				printf("Error in write. \n");
			}

			//if(strstr(sendline, escapechar) != NULL) // 종료 문자열
			if(strstr(string, escapechar) != NULL)
			{
				printf("Good byte.\n");
				close(s);
				while(1);    //자식프로세서가 죽을때까지 블로킹
			}
		}
	}

	else if(fork_ret == 0)
	{
		// 자식 프로세스는 서버로부터 수신된 메시지를 화면에 출력
		while(1)
		{
			if((size = read(s, recvline, MAXLINE)) < 0)
			{
				printf("Error if read. \n");
				close(s);
				return 0;
			}
	
			recvline[size] = '\0';
			if(strstr(recvline, escapechar)!=NULL) // 종료 문자열
			{
				write(s, escapechar, strlen(escapechar));
				break;
			}

			printf("%s", recvline); // 화면 출력
		}
	}
	
	close(s);
}

void* loop3(void *data) // iBeacon
{

}

void* loop4(void *data) // PIR
{

}

int main(int argc, char *argv[])
{
	//init server client
	static struct sockaddr_in server_addr;
	struct sigaction act;
	act.sa_handler = z_handler;

	if(argc != 3)
	{
		printf("Usage : %s serverIP serverPORT \n", argv[0]);
		exit(0);
	}

	// 소켓 생성
	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Client : can't open stream socket.\n");
		exit(0);
	}
	// 소켓 주소 구조체에 접속할 서버 주소 세팅
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	sigaction(SIGCHLD, &act, 0);
	// 서버에 연결 요청
	if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("Client : can't connect to server.\n");
		exit(0);
	}
	// end init

	int thr_id;
	pthread_t p_thread[4];
	int status;
	int a = 1;

	thr_id = pthread_create(&p_thread[0], NULL, loop1, (void *)&a);
	thr_id = pthread_create(&p_thread[1], NULL, loop2, (void *)&a);
	thr_id = pthread_create(&p_thread[2], NULL, loop3, (void *)&a);
	thr_id = pthread_create(&p_thread[3], NULL, loop4, (void *)&a);
	

	pthread_join(p_thread[0], (void *) &status);
	pthread_join(p_thread[1], (void *) &status);
	pthread_join(p_thread[2], (void *) &status);
	pthread_join(p_thread[3], (void *) &status);

	status = pthread_mutex_destroy(&mutex);
	printf("code = %d", status);
	printf("programing is end");
	return 0;
}
