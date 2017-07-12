#include "hal/board.h"
#include <drivers/gpiotbl.h>
#include <hal/arch.h>
#include <hal/irq.h>
#include <gxx/debug/delay.h>

#include <asm/diag.h>

void emergency_stop() __attribute__((weak));
void emergency_stop() {};

void board_init() {
	arch_init();
	usart0_diag_init();

	pinnum_settings(RED_LED, GPIO_MODE_OUTPUT);
	pinnum_settings(YELLOW_LED, GPIO_MODE_OUTPUT);
	pinnum_settings(GREEN_LED, GPIO_MODE_OUTPUT);

	pinnum_set_level(RED_LED,0);
	pinnum_set_level(YELLOW_LED,0);
	pinnum_set_level(GREEN_LED,0);
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
			global_irqs_disable();
			gpio_settings(GPIOB, (1<<7), GPIO_MODE_OUTPUT);
			while(1) {
				pinnum_tgl_level(RED_LED);
				debug_delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}