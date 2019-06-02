#include <stdio.h>
#include <stdlib.h>
#include <math.h>





int main()
{
	float TXpower = 0;
        float RSSI = 0;
	double D = 0;
	int n=2;

	printf("Txpower,RSSI input \n");
	scanf("%f %f", &TXpower, &RSSI);

	D= pow(10,((TXpower-RSSI)/(20)));

	printf("%.8f\n", D);
}

