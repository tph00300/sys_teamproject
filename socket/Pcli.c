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
char string[4] = "0 1";
int check = 1;
void* loop1(void *data)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);

		printf("LOOP 1 is free!");
		printf("string : %s", string);
		string[2] = '0'; 
		check = 0;
		printf("string : %s", string);

		pthread_mutex_unlock(&mutex);
	
		sleep(2);

	}
}

void* loop2(void *data)
{
	fork_ret = fork();
	if(fork_ret > 0)
	{
		// 부모 프로세스는 키보드 입력을 서버로 송신
		//while(fgets(sendline, MAXLINE, stdin) != NULL)
		while(check == 0)
		{
			//size = strlen(sendline);
			size = strlen(string);
			//if(write(s, sendline, strlen(sendline)) != size)
			if(write(s, string, strlen(string)) != size)
			{
				printf("Error in write. \n");
			}

			if(strstr(sendline, escapechar) != NULL) // 종료 문자열
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
	pthread_t p_thread[2];
	int status;
	int a = 1;

	thr_id = pthread_create(&p_thread[0], NULL, loop1, (void *)&a);
	thr_id = pthread_create(&p_thread[1], NULL, loop2, (void *)&a);
	

	pthread_join(p_thread[0], (void *) &status);
	pthread_join(p_thread[1], (void *) &status);

	status = pthread_mutex_destroy(&mutex);
	printf("code = %d", status);
	printf("programing is end");
	return 0;
}
