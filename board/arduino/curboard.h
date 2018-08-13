#ifndef CRIUS_BOARD_H
#define CRIUS_BOARD_H

#define RED_LED 13

#include <sys/cdefs.h>
#include <genos/hal/gpio.h>

#include <arch/gpio.h>
#include <arch/usart.h>
//#include <arch/i2c.h>

#if defined(BOARD_ARDUINO_UNO)
#	define SYSLED_GPIO GPIOB
#	define SYSLED_PIN 5
#elif defined (BOARD_ARDUINO_MEGA)
#	define SYSLED_GPIO GPIOB
#	define SYSLED_PIN 7
#else 
#	error "undefined board"
#endif

namespace board {
	extern arch::gpio::pin led;
	extern arch::usart usart0;
	//extern arch::i2c i2c;
}

__BEGIN_DECLS

void board_init();
void board_shutdown(arch_shutdown_mode_t mode) __attribute__((noreturn));

void emergency_stop();

__END_DECLS

#endif