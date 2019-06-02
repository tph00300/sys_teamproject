#include <stdio.h>

#include "buzzer.h"
#include "flame.h"
#include "gas.h"

int main ()
{
	// check buzzer
	printf("check buzzer\n");
	printf("init_buzzer\n");
	int check = init_buzzer();
/*
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

*/
	//check gas
	printf("check gas\n");
	printf("init_gas\n");
	check = init_gas();

	if(check != 0)
	{
		printf("end program(ERR)\n");
		return 0;
	}

	for(int i = 0; i < 25; i++)
	{
		int result = read_gas();
		printf("gas result : %d\n", result);
	}

	check = close_gas();

	if(check != 0)
	{
		printf("end program(ERR)\n");
		return 0;
	}
	
	return 0;
}
