#include <hal/board.h>
#include <hal/irq.h>

#include <drivers/gpiotbl.h>
#include <debug/delay.h>
#include <debug/dprint.h>

void board_init() {
	arch_init();

	gpio_settings(GPIOB, (1<<7), GPIO_MODE_OUTPUT);

	pinnum_set_level(RED_LED,1);
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
			debug_print("arch_shutdown"); dln();
			while(1) {
				pinnum_tgl_level(RED_LED);
				debug_delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}

__attribute__((weak)) void emergency_stop() {}