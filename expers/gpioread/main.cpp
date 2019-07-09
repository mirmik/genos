#include <hal/board.h>
#include <systime/systime.h>

#include <util/cpu_delay.h>

#include <board/pinout.h>
#include <drivers/gpio/gpio.h>

int main() 
{
	board_init();

	PINOUT[2].mode(GPIO_MODE_INPUT | GPIO_MODE_IN_PULL_UP);

	while(1) 
	{
		dprln(PINOUT[2].get());

		gpio_pin_toggle(&board_led);
		cpu_delay(50000);
	}
}
