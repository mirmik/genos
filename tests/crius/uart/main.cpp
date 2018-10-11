#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <gxx/debug/dprint.h>
#include <gxx/debug/delay.h>

#include <sched/sched.h>
#include <sched/timer.h>

//#include <crow/tower.h>

//#include <drivers/crow/uartgate.h>

#include <periph/irqdefs.h>
#include <drivers/serial/avr_usart.h>
//#include <drivers/serial/uartring.h>

struct avr_usart uart0;

int main() {
	//int sts;

	board_init();
	schedee_manager_init();

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);

	gpio_set_level(RED_LED_GPIO, RED_LED_MASK, 1);
	gpio_set_level(GREEN_LED_GPIO, GREEN_LED_MASK, 1);

	struct uart_params uparams = { 115200, UART_PARITY_NONE, 1, 8 };
	usart_regs_sendbyte(USART0, 'A');

	avr_usart_init(&uart0, USART0, ATMEGA_IRQ_U0RX);

	__schedule__();
}

void __schedule__() {
	while(1) {
	//	crow_onestep();
		timer_manager();
		schedee_manager();
	}
}


uint16_t crow_millis() {
	return millis();
}