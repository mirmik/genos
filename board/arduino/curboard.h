#ifndef CRIUS_BOARD_H
#define CRIUS_BOARD_H

#define RED_LED 13

#include <sys/cdefs.h>
#include <drivers/gpio/gpio.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/avr_usart.h>

#if defined(BOARD_ARDUINO_UNO)
#	define RED_LED_GPIO GPIOB
#	define RED_LED_MASK 1<<5
#	define RED_LED_PIN     5

#elif defined (BOARD_ARDUINO_MEGA)
#	define RED_LED_GPIO GPIOB
#	define RED_LED_MASK 1<<7
#	define RED_LED_PIN     7

#elif defined (BOARD_CRIUS_AIOP2)
#	define RED_LED_GPIO GPIOB
#	define RED_LED_MASK 1<<7
#	define RED_LED_PIN     7

#	define YELLOW_LED_GPIO GPIOC
#	define YELLOW_LED_MASK 1<<6
#	define YELLOW_LED_PIN     6

#	define GREEN_LED_GPIO GPIOC
#	define GREEN_LED_MASK 1<<7
#	define GREEN_LED_PIN     7

#else 
#	error "undefined board"
#endif

#define SYSLED_GPIO RED_LED_GPIO
#define SYSLED_MASK RED_LED_MASK
#define SYSLED_PIN  RED_LED_PIN

extern struct gpio_pin board_led;
extern struct avr_usart_device usart0;

__BEGIN_DECLS

//void board_init();
//void board_shutdown(arch_shutdown_mode_t mode) __attribute__((noreturn));

void emergency_stop();
void board_init();

__END_DECLS

#ifdef __cplusplus

namespace board 
{
	//extern gpio_pin sysled;
//	extern genos::drivers::avr_usart usart0;
}

#endif

#endif