#include "board.h"
#include <drivers/gpio.h>
#include <hal/arch.h>
#include <hal/irq.h>
#include <debug/delay.h>

void board_init() {
	arch_init();

	gpio_settings(GPIOB, (1<<7), GPIO_MODE_OUTPUT);

	pin_set_level(RED_LED,1);
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
			global_irq_disable();
			gpio_settings(GPIOB, (1<<7), GPIO_MODE_OUTPUT);
			while(1) {
				pin_tgl_level(RED_LED);
				debug_delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}