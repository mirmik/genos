#include <hal/board.h>
#include <asm/irq.h>
#include <systime/systime.h>

#include <drivers/gpio/pin.h>
//#include <drivers/serial/avr_usart.h>
#include <periph/irqdefs.h>

#include <igris/dprint/dprint.h>

GPIO_PIN(board_led, SYSLED_GPIO, SYSLED_PIN);

AVR_USART_DEVICE_DECLARE(usart0, USART0, ATMEGA_IRQ_U0RX);

void board_init() {
	arch_init();
	//arch_uart_init();	
	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT);
	gpio_pin_write(&board_led, 1);
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