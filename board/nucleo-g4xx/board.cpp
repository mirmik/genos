#include <hal/board.h>
#include <hal/arch.h>
#include "curboard.h"

#include <periph/map.h>

#include <asm/stm32_rcc.h>
#include <asm/stm32_gpio.h>

namespace board 
{
	gpio_pin sysled(SYSLED_GPIO, SYSLED_PINMASK);
}

void board_init() 
{
	arch_init();

	stm32_rcc_enable_gpio(GPIOA);
	stm32_gpio_set_output(GPIOA, 0xFF);
	stm32_gpio_set_level(GPIOA, 0xFF, 1);
}
