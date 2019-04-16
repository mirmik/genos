#include <drivers/gpio/gpio.h>
#include <hal/board.h>

#include <asm/rcc.h>
#include <periph/map.h>

#include <util/delay.h>
#include <hal/irq.h>

#include <systime/systime.h>

#include <asm/arch.h>

int main() 
{
	board_init();
	
	rcc_enable_gpio(GPIOD);
	gpio_settings(GPIOD, 0b1111 << 12, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIOD, 0b1111 << 12, 0);

	irqs_enable();

	while(1) 
	{
		delay(1000);
		//cpu_delay(800000);
		gpio_set_level(GPIOD, 0b1111 << 12, 0);		
		//cpu_delay(800000);
		delay(1000);
		gpio_set_level(GPIOD, 0b1111 << 12, 1);
	}
}