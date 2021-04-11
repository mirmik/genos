#include <hal/board.h>
#include <asm/irq.h>
#include <systime/systime.h>

#include <drivers/gpio/avr_gpio.h>
#include <periph/map.h>

#include <igris/dprint/dprint.h>

avr_gpio_pin board_sysled(SYSLED_GPIO, SYSLED_PIN);

AVR_USART_DEVICE_DECLARE(usart0, USART0, ATMEGA_IRQ_U0RX);

void board_init() {
	arch_init();

	board_sysled.setup(GPIO_MODE_OUTPUT);
	board_sysled.set(1);
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