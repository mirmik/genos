#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <gxx/debug/dprint.h>
#include <gxx/debug/delay.h>

#include <sched/sched.h>
#include <sched/schedee.h>
#include <sched/timer.h>

#include <crow/tower.h>

#include <drivers/crow/uartgate.h>

#include <drivers/serial/avr_usart.h>
#include <periph/irqdefs.h>

#include <gxx/util/hexer.h>

#include <sched/api.h>

//#include <drivers/serial/uartring.h>

avr_usart uart0;
crow_uartgate uartgate;

void* mainproc(void*) 
{
	uint8_t raddr[1];
	hexer_s(raddr, 1, ".10");

	while(1) 
	{
		crow_send(raddr, 1, "HelloWorld", 10, 0, 0, 200);
		msleep(1000);
	}
}

int main()
{
	board_init();
	schedee_manager_init();

	uart0.init(USART0, ATMEGA_IRQ_U0RX);

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);

	gpio_set_level(RED_LED_GPIO, RED_LED_MASK, 1);
	gpio_set_level(GREEN_LED_GPIO, GREEN_LED_MASK, 1);

	uart0.setup(115200, UART_PARITY_NONE, 8, 1);
	uart0.enable(true);

	crow_uartgate_init(&uartgate, &uart0);

	irqs_enable();


	schedee_run(create_cooperative_schedee(mainproc, nullptr, 200));

	__schedule__();
}

void __schedule__()
{
	while (1)
	{
		crow_onestep();
		timer_manager();
		schedee_manager();
	}
}

uint16_t crow_millis()
{
	return millis();
}