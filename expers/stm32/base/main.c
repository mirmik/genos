#include <hal/board.h>
#include <hal/irq.h>

#include <asm/arch.h>
#include <asm/usart.h>
#include <asm/rcc.h>

#include <drivers/gpio/gpio.h>

#include <periph/map.h>

#include <util/delay.h>
#include <systime/systime.h>
#include <igris/dprint.h>

int main() 
{
	board_init();
	
	rcc_enable_gpio(GPIOD);
	
	gpio_settings(GPIOD, 0b1111 << 12, GPIO_MODE_OUTPUT);
	gpio_write(GPIOD, 0b1111 << 12, 1);

	irqs_enable();

	while(1) 
	{
		delay(1000);
		dprln("HelloWorld");
		gpio_write(GPIOD, 0b1111 << 12, 0);		
		delay(1000);
		//gpio_write(GPIOD, 0b1111 << 12, 1);
		
		//dprln(board_green_led.mask);
		//dprln(board_green_led.mask);
		//dprln(board_yellow_led.mask);
		//dprln(board_blue_led.mask);

		gpio_pin_write(&board_red_led, 1);
		gpio_pin_write(&board_green_led, 1);
		gpio_pin_write(&board_yellow_led, 1);
		gpio_pin_write(&board_blue_led, 1);
	}
}