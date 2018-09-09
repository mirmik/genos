#ifndef CRIUS_BOARD_H
#define CRIUS_BOARD_H

#define RED_LED 13

#include <sys/cdefs.h>
#include <drivers/gpio/gpio.h>

#if defined(BOARD_ARDUINO_UNO)
#	define REDLED_GPIO GPIOB
#	define REDLED_PIN 5
#elif defined (BOARD_ARDUINO_MEGA)
#	define REDLED_GPIO GPIOB
#	define REDLED_PIN 7
#else 
#	error "undefined board"
#endif

extern struct gpio_pin_head led_red;

__BEGIN_DECLS

//void board_init();
//void board_shutdown(arch_shutdown_mode_t mode) __attribute__((noreturn));

void emergency_stop();

__END_DECLS

#endif