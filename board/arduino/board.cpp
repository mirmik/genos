#include <genos/hal/board.h>
#include <arch/irqs.h>
#include <periph/irqdefs.h>

#include <genos/time/systime.h>
#include <gxx/debug/dprint.h>

namespace board {
	arch::gpio::pin led(SYSLED_GPIO, SYSLED_PIN);
	arch::usart usart0(USART0, ATMEGA_IRQ_U0RX);
}

void board_init() {
	arch_init();
	
	board::led.mode(genos::hal::gpio::output);
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
			board::led.mode(genos::hal::gpio::output);
			
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