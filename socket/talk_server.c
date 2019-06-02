//talk_server.c
 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAXLINE 512

void z_handler();// 시그널 처리 함수

char *escapechar = "exit"; // 종료 문자열

int main(int argc, char *argv[])
{
  int server_sock;
  int client_sock;
  int clntlen;
  int num;
  char sendline[MAXLINE];
  char recvline[MAXLINE];
  int size;
  pid_t fork_ret;
  struct sockaddr_in client_addr;
  struct sockaddr_in server_addr;
  int state;
  struct sigaction act;
  act.sa_handler = z_handler;

  if(argc!=2)
  {
    printf("Usage : %s PORT \n", argv[0]);
    exit(0);
  }
  // 소켓 생성
  if((server_sock = socket(PF_INET, SOCK_STREAM, 0)) <0)
  {
    printf("Server : can't open stream socket. \n");
    exit(0);
  }
  // 소켓 주소 구조체에 주소 세팅
  bzero((char *)&server_addr, sizeof(server_addr)); // 소켓 주소 구조체 초기화
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(atoi(argv[1]));

  sigaction(SIGCHLD, &act, 0);

  // 소켓에 서버 주소 연결
  if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
  {
    printf("Server : can't bind local address.\n");
    exit(0);
  }
  printf("Server started. \nWaiting for client.. \n");
  listen(server_sock, 1);

  // 클라이언트의 연결요청 수락
  clntlen = sizeof(client_addr);
  if((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &clntlen)) <0)
  {
    printf("Server : failed in accepting. \n");
    exit(0);
  }
  if((fork_ret = fork()) > 0)
  {
    // 부모 프로세스는 키보드 입력을 클라이언트로 송신
    while(fgets(sendline, MAXLINE, stdin)!=NULL)
    {
      size = strlen(sendline);
      if(write(client_sock, sendline, strlen(sendline)) != size)
      {
        printf("Error in write. \n");        
      }
      if(strstr(sendline, escapechar) != NULL) // 종료 문자열 입력시 처리
      {
        printf("Good bye.\n");
        close(client_sock);
        while(1);    //자식프로세서가 죽을때까지 블로킹 
      }
    }
  }
  else if(fork_ret == 0)
  {
    // 자식 프로세스는 클라이언트로부터 수신된 메시지를 화면에 출력
    while(1)
    {
      if((size = read(client_sock, recvline, MAXLINE)) < 0)
      {
        printf("Error if read. \n");
        close(client_sock);
        exit(0);
      }
      recvline[size] = '\0';
      if(strstr(recvline, escapechar) != NULL) // 종료 문자열 입력시 처리
      {
        write(client_sock, escapechar, strlen(escapechar));
        break;
      }
      printf("%s", recvline); // 화면 출력
    }
  }
  close(server_sock);
  close(client_sock);

  return 0;
}
void z_handler()
{
  int state;
  waitpid(-1, &state, WNOHANG);
  exit(0);

  return ;
}


//출처: https://yms2047.tistory.com/entry/양방향-통신11-대화 [Oh~ Happy!]
