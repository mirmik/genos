#include <hal/board.h>
#include <hal/irq.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>

#include <sched/sched.h>
#include <sched/schedee.h>
#include <sched/timer.h>

#include <crow/tower.h>

#include <drivers/crow/uartgate.h>
#include <drivers/serial/avr_usart.h>
#include <periph/irqdefs.h>

#include <sched/api.h>


//#include <drivers/serial/uartring.h>

avr_usart uart0(USART0, ATMEGA_IRQ_U0RX);
crow_uartgate uartgate;

void* mainproc(void*) 
{	
	uint8_t raddr[1];
	hexer_s(raddr, 1, "#F4");

	gpio_set_level(GREEN_LED_GPIO, GREEN_LED_MASK, 0);
	//msleep(1000);
	while(1) 
	{
		crow::send(raddr, 1, "HelloWorld", 10, 0, 0, 200);
		msleep(1000);
	}
}

void user_incoming(crow::packet* pack) 
{
	gpio_toggle(GREEN_LED_GPIO, GREEN_LED_MASK);
	crow::release(pack);
}


extern "C" void dos() 
{
	gpio_toggle(RED_LED_GPIO, RED_LED_MASK);
}

int main()
{
	board_init();

	schedee_manager_init();

	gpio_settings(RED_LED_GPIO, RED_LED_MASK, GPIO_MODE_OUTPUT);
	gpio_settings(GREEN_LED_GPIO, GREEN_LED_MASK, GPIO_MODE_OUTPUT);

	gpio_set_level(RED_LED_GPIO, RED_LED_MASK, 1);
	gpio_set_level(GREEN_LED_GPIO, GREEN_LED_MASK, 1);

	uart0.setup(115200, UART_PARITY_NONE, 8, 1);
	uart0.enable(true);

	uartgate.init(&uart0);
	crow::user_incoming_handler = user_incoming;

	irqs_enable();

	//delay(1);

	schedee_run(create_cooperative_schedee(mainproc, nullptr, 256));

	__schedule__();
}

void __schedule__()
{
	while (1)
	{
		crow::onestep();
		timer_manager();
		schedee_manager();
	}
}

uint16_t crow::millis()
{
	return ::millis();
}