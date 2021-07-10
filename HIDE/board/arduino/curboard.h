#ifndef CRIUS_BOARD_H
#define CRIUS_BOARD_H

#define RED_LED 13

#include <periph/map.h>
#include <igris/compiler.h>

#if defined(BOARD_ARDUINO_UNO)
#	define RED_LED_GPIO GPIOB
#	define RED_LED_MASK 1<<5
#	define RED_LED_PIN     5

#elif defined(BOARD_ARDUINO_NANO)
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

#define SYSUSART USART0

__BEGIN_DECLS

void board_init();
void board_test();

__END_DECLS

#ifdef __cplusplus

#endif

#endif