#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>
#include <igris/dprint.h>

#include <sched/sched.h>
#include <sched/timer.h>
#include <sched/api.h>

#include <periph/irqdefs.h>
#include <drivers/serial/avr_usart.h>
#include <drivers/serial/uartring.h>

#include <utility/getty.h>

struct avr_usart uart0;
genos::uartring_device serial0;

struct getty_context terminal_ttyS0_context(&serial0);

int main() {
	board_init();
	schedee_manager_init();

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);

	gpio_set_level(RED_LED_GPIO, RED_LED_MASK, 1);
	gpio_set_level(GREEN_LED_GPIO, GREEN_LED_MASK, 1);

	//struct uart_params uparams = { 115200, UART_PARITY_NONE, 1, 8 };
	uart0.init(USART0, ATMEGA_IRQ_U0RX);
	uart0.setup(115200);
	uart0.enable(true);

	serial0.init(&uart0, "ttyS0");
	
	schedee_run (create_autom_schedee(nologin_getty, &terminal_ttyS0_context));

	//schedee_run(create_cooperative_schedee(maintask, NULL, 512));

	irqs_enable();
	__schedule__();
}

void __schedule__() {
	while(1) {
		timer_manager();
		genos::schedee_manager::step();
	}
}


uint16_t crow_millis() {
	return millis();
}