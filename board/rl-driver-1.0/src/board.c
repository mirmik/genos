#include <drivers/gpio.h>

#include <genos/hal/board.h>
#include <genos/hal/arch.h>
#include <genos/hal/irq.h>

#include <gxx/debug/delay.h>
#include <gxx/debug/dprint.h>

#include <asm/rcc.h>

void board_init() {
	arch_init();
	arch_external_generator_init();

	rcc_enable_gpio(GPIOD);
	gpio_settings(GPIOD, (1<<12 | 1<<13 | 1<<14 | 1<<15), GPIO_MODE_OUTPUT);
	gpio_set_level(GPIOD, (1<<12 | 1<<13 | 1<<14 | 1<<15), 1);
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
//			gpio_settings(GPIOB, (1<<7), GPIO_MODE_OUTPUT);
			debug_print("arch_shutdown"); dln();
			while(1) {
//				pin_tgl_level(RED_LED);
				debug_delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}

__attribute__((weak)) void emergency_stop() {}