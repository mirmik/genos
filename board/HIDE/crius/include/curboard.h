#ifndef CRIUS_BOARD_H
#define CRIUS_BOARD_H

#define RED_LED 13

#include <sys/cdefs.h>
#include <hal/gpio.h>

#include <arch/gpio.h>
#include <arch/usart.h>
//#include <arch/i2c.h>

namespace board {
	extern arch::gpio::pin red_led; //GPIOB, 7
	extern arch::gpio::pin green_led; //GPIOC, 7
	extern arch::gpio::pin yellow_led; //GPIOC, 6
	extern arch::usart usart0;
	//extern arch::i2c i2c;
}

__BEGIN_DECLS

void board_init();
void board_shutdown(arch_shutdown_mode_t mode) __attribute__((noreturn));

void emergency_stop();

__END_DECLS

#endif