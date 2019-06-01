#include <bcm2835.h>
#include <stdio.h>
#define PIN_PWM RPI_GPIO_P1_12
#define PWM_CHANNEL 0
#define RANGE 1024

int main (int argc, char **argv) {
	if(!bcm2835_init())
		return 1;

	bcm2835_gpio_fsel(PIN_PWM,BCM2835_GPIO_FSEL_ALT5);
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
	bcm2835_pwm_set_mode(PWM_CHANNEL,1,1);
	bcm2835_pwm_set_range(0,RANGE);

	int direction = 1;
	int data = 1;
	while (1) {
		data = 262;
		bcm2835_pwm_set_data(PWM_CHANNEL,data);
		//bcm2835_delay(50);
	}

	bcm2835_close();
	return 0;
}
