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
#include <drivers/serial/uartring.h>

struct avr_usart uart0;
struct uartring_device serial0;

int main() {
	board_init();
	schedee_manager_init();

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);

	gpio_set_level(RED_LED_GPIO, RED_LED_MASK, 1);
	gpio_set_level(GREEN_LED_GPIO, GREEN_LED_MASK, 1);

	struct uart_params uparams = { 115200, UART_PARITY_NONE, 1, 8 };
	//usart_regs_sendbyte(USART0, 'A');

	uart0.init(USART0, ATMEGA_IRQ_U0RX);
	uart0.setup(&uparams);
	uart0.enable(true);

	serial0.init(&uart0, "ttyS0");
	
	irqs_enable();


	//uart0.putc('D');
	int ret1= serial0.write("HelloWorld\n", 11);
	int ret2= serial0.write("HelloWorld\n", 11);
	int ret3= serial0.write("HelloWorld\n", 11);
	int ret4= serial0.write("HelloWorld\n", 11);

	debug_delay(100000);
	dprln(ret1);
	dprln(ret2);
	dprln(ret3);
	dprln(ret4);

	__schedule__();
}

void __schedule__() {
	while(1) {
		timer_manager();
		schedee_manager();
	}
}


uint16_t crow_millis() {
	return millis();
}