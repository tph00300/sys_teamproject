#include "buzzer.h"
#include <stdio.h>

int main ()
{
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
			printf("end program\n");
			return 0;
		}
	}
	
	return 0;
}
