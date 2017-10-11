#include <hal/board.h>
#include <hal/irqs.h>
#include <hal/gpio.h>

#include <genos/systime.h>
#include <gxx/debug/dprint.h>

void board_init() {
	arch_init();
	hal::gpio::pin led(GPIOB, 7);

	led.mode(hal::gpio::output);
	led.set();
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
			hal::irqs::disable();

			hal::gpio::pin led(GPIOB, 7);
			led.mode(hal::gpio::output);
			
			dprln("arch_shutdown");
			while(1) {
				led.tgl();
				systime::delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}

__attribute__((weak)) void emergency_stop() {}