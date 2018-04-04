#include <genos/hal/board.h>
#include <genos/hal/arch.h>

#include <periph/map.h>
#include <drivers/gpio.h>

void board_init() {
	*RCC_AHB1ENR |= 1 << 3;
	//gpio_settings(GPIOD, 1<<12 | 1<<13 | 1<<14 | 1<<15, GPIO_MODE_OUTPUT);
	gpio_settings(GPIOB, (1<<13), GPIO_MODE_OUTPUT);
}