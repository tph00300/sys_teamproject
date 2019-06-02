#include <stdio.h>

#include "buzzer.h"
#include "flame.h"
#include "gas.h"
#include "led.h"
#include "pir.h"

int main ()
{

	// check buzzer
	printf("check buzzer\n");
	printf("init_buzzer\n");
	int check = init_buzzer();

	if(check != 0)
	{
		printf("end program(ERR)\n");
		return 0;
	}

	while(1)
	{	
		int n;
		printf("input : ");
		scanf("%d", &n);

		if(n == 0)
		{
			printf("on_buzzer");
			on_buzzer();
		}

		else if (n == 1)
		{
			printf("off_buzzer\n");
			off_buzzer();
		}

		else
		{
			printf("end check buzzer\n");
			break;
		}
	}


	//check flame
	printf("check flame\n");
	printf("init_MCP3008\n");
	check = init_MCP3008();

	if(check != 0)
	{
		printf("end program(ERR)\n");
		return 0;
	}

	for(int i = 0; i < 25; i++)
	{
		int result = read_flame();
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

	for(int i = 0; i < 20; i++)
	{
		int result = read_gas();
		printf("gas result : %d\n", result);
	}

	close_gas();
	*/

	//check led
	printf("check led\n");
	printf("init_led");
	init_led();

	if(check != 0)
	{
		printf("end program(ERR)\n");
		return 0;
	}

	on_led();
	sleep(2);
	off_led();
	sleep(2);
	on_led();
	sleep(2);
	off_led();
	sleep(2);
	
	//check pir
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
	return 0;
}
