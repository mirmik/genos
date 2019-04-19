#ifndef STM32F4DISCOVERY_V2_BOARD
#define STM32F4DISCOVERY_V2_BOARD

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>

#define HSE_FREQ 8000000

extern struct gpio_pin board_led;

__BEGIN_DECLS

void board_init();

__END_DECLS

#endif