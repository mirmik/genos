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

#include <asm/nvic.h>
#include <periph/map.h>

char serial2_rxbuf[128];
char serial2_txbuf[128];

int main() 
{
	board_init();

	genos::drivers::stm32_usart usart2(USART2, STM32_IRQ_USART2);
	usart2.setup(115200, 'n', 8, 1);

	genos::drivers::uartring serial2(&usart2, serial2_rxbuf, 128, serial2_txbuf, 128);

	dprint_dump_nvic();	
	
	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT);
	gpio_pin_write(&board_led, 1);

	irqs_enable();

	while(1) 
	{
		serial2.println("HelloWorld");

		delay(1000);	
		gpio_pin_write(&board_led, 0);
		delay(1000);
		gpio_pin_write(&board_led, 1);
	}
}