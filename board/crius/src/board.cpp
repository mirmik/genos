#include <genos/hal/board.h>
#include <arch/irqs.h>

#include <genos/time/systime.h>
#include <gxx/debug/dprint.h>

namespace board {
	arch::gpio::pin red_led(GPIOB, 7);
	arch::gpio::pin green_led(GPIOC, 7);
	arch::gpio::pin yellow_led(GPIOC, 6);
	arch::usart usart0(usart0_data);
//	arch::i2c i2c;
}

void board_init() {
	arch_init();
	systime::frequency = 16000000 / 256 / 64;

	board::red_led.mode(genos::hal::gpio::output);
	board::green_led.mode(genos::hal::gpio::output);
	board::yellow_led.mode(genos::hal::gpio::output);
	board::red_led.set();
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
			board::red_led.mode(genos::hal::gpio::output);
			
			dprln("arch_shutdown");
			while(1) {
				board::red_led.tgl();
				systime::delay(100);
			}
		break;
	};
	arch_shutdown(mode);	
}

__attribute__((weak)) void emergency_stop() {}