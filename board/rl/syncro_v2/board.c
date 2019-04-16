#include <hal/board.h>
#include <asm/arch.h>
#include "curboard.h"

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <asm/rcc.h>
#include <periph/map.h>

void board_init() 
{
	arch_init();

	struct stm32_pll_settings pll_settings = {
		.Mkoeff = 8,
		.Nkoeff = 168,
		.Pkoeff = 2,
		.Qkoeff = 7
	}; //84 000 000 Гц

	stm32_external_generator_setup(&pll_settings);
	stm32_systick_config(84000);
	systime_set_frequency(1000);

	rcc_enable_usart(USART2);
	rcc_enable_usart(USART6);

	rcc_enable_gpio(GPIOD);
}