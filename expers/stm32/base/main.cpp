#include <hal/board.h>
#include <hal/irq.h>

#include <asm/arch.h>
#include <asm/rcc.h>

#include <drivers/gpio/gpio.h>

#include <periph/map.h>
#include <periph/irqdefs.h>

#include <util/delay.h>
#include <systime/systime.h>
#include <igris/dprint.h>

#include <drivers/serial/stm32_usart.h>
#include <drivers/serial/uartring.h>

#include <nos/print.h>

char serial2_rxbuf[128];
char serial2_txbuf[128];

int main() 
{
	board_init();

	genos::drivers::stm32_usart usart2(USART2, STM32_IRQ_USART2);
	usart2.setup(115200, 'n', 8, 1);

	genos::drivers::uartring serial2(&usart2, serial2_rxbuf, 128, serial2_txbuf, 128);
	
	rcc_enable_gpio(GPIOD);
	
	gpio_settings(GPIOD, 0b1111 << 12, GPIO_MODE_OUTPUT);
	gpio_write(GPIOD, 0b1111 << 12, 1);

	irqs_enable();

	while(1) 
	{
		delay(1000);

		serial2.print("HelloWorld\n");
	
		gpio_write(GPIOD, 0b1111 << 12, 0);		
		delay(1000);
		//gpio_write(GPIOD, 0b1111 << 12, 1);
		
		//dprln(board_green_led.mask);
		//dprln(board_green_led.mask);
		//dprln(board_yellow_led.mask);
		//dprln(board_blue_led.mask);

		gpio_pin_write(&board_red_led, 1);
		gpio_pin_write(&board_green_led, 1);
		//gpio_pin_write(&board_yellow_led, 1);
		gpio_pin_write(&board_blue_led, 1);
	}
}