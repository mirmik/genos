#include <hal/board.h>
#include <arch/irqs.h>

#include <genos/systime.h>
#include <gxx/debug/dprint.h>

namespace board {
	arch::gpio::pin led(GPIOB, 7);
	arch::usart usart0(usart0_data);
//	arch::i2c i2c;
}

void board_init() {
	arch_init();

	board::led.mode(hal::gpio::output);
	board::led.set();
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
			arch::irqs::disable();
			board::led.mode(hal::gpio::output);
			
			dprln("arch_shutdown");
			while(1) {
				board::led.tgl();
				systime::delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}

__attribute__((weak)) void emergency_stop() {}