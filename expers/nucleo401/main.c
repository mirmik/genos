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

#include <drivers/serial/uart.h>
#include <drivers/serial/stm32_usart.h>
#include <drivers/serial/uartring.h>

UARTRING_DECLARE(serial2, &usart2, 16, 128);

int main() 
{
	board_init();

	uart_device_setup(&usart2, 115200, 'n', 8, 1);
	uartring_begin(&serial2, &usart2.dev);

	//dprint_dump_nvic();	
	
	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT);
	gpio_pin_write(&board_led, 1);

	//scheduler_init();
	irqs_enable();

	while(1) 
	{
		serial2.cdev.c_ops->write(&serial2.cdev, "HelloWorld\n", 11, 0);
		
		delay(1000);
		gpio_pin_write(&board_led, 0);
		delay(1000);
		gpio_pin_write(&board_led, 1);
	}
}