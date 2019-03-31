#include <hal/board.h>
#include <arch/irq.h>
#include <systime/systime.h>

#include <drivers/gpio/pin.h>
#include <drivers/serial/avr_usart.h>
#include <periph/irqdefs.h>

#include <igris/dprint/dprint.h>

namespace board {
	gpio_pin sysled(SYSLED_GPIO, SYSLED_MASK);
	avr_usart usart0(USART0, ATMEGA_IRQ_U0RX);
}

void board_init() {
	arch_init();
	//arch_uart_init();	
	board::sysled.mode(GPIO_MODE_OUTPUT);
	board::sysled.set(1);
}

void board_shutdown(arch_shutdown_mode_t mode) {
	emergency_stop();
	switch(mode)
	{
		case ARCH_SHUTDOWN_MODE_HALT:
		break;
		case ARCH_SHUTDOWN_MODE_REBOOT:
		break;
		case ARCH_SHUTDOWN_MODE_ABORT:
			irqs_disable();
			//board::led.mode(genos::hal::gpio::output);
			
			debug_print("arch_shutdown\n");
			while(1) {
			//	board::led.set();
				while(1);
			}
		break;
	};
	arch_shutdown(mode);	
}

__attribute__((weak)) void emergency_stop() {}