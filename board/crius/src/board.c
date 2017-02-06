#include "crius_aiop_pro.h"
#include <drivers/gpio.h>
#include <hal/arch.h>

void board_init() {
	arch_init();

	gpio_settings(GPIOB, (1<<7), GPIO_MODE_OUTPUT);
	gpio_settings(GPIOC, (1<<7) | (1<<6), GPIO_MODE_OUTPUT);

	pin_set_level(RED_LED,1);
	pin_set_level(YELLOW_LED,1);
	pin_set_level(GREEN_LED,1);
}