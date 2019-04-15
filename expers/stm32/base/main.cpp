#include <drivers/gpio/gpio.h>
#include <hal/board.h>

#include <arch/rcc.h>

int main() 
{
	board_init();

	rcc_enable_gpio(GPIOD);
	gpio_settings(GPIOD, 0xFFFF, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIOD, 0xFFFF, 1);

	while(1) 
	{

	}
}