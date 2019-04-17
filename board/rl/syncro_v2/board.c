#include <hal/board.h>
#include "curboard.h"

#include <asm/arch.h>
#include <asm/rcc.h>
#include <asm/usart.h>

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <periph/map.h>

void board_init() 
{
	arch_init();
	stm32_declared_freq[DECLARED_FREQ_HSE_NO] = 8000000;

	struct stm32_pll_settings pll_settings = {
		.Mkoeff = 8,
		.Nkoeff = 168,
		.Pkoeff = 2,
		.Qkoeff = 7
	}; //84 000 000 Гц

	stm32_init_pll_clocking(&pll_settings);
	stm32_systick_config(84000);
	systime_set_frequency(1000);

	stm32_declared_freq[DECLARED_FREQ_PLL_NO] = 84000000; 
	stm32_declared_freq[DECLARED_FREQ_APB1_NO] = 84000000;
	stm32_declared_freq[DECLARED_FREQ_APB2_NO] = 42000000;

	rcc_enable_usart(USART2);
	rcc_enable_usart(USART6);
	rcc_enable_gpio(GPIOA);
	rcc_enable_gpio(GPIOD);

	stm32_usart_setup(USART2, 115200, 'n', 8, 1);
	stm32_diag_init(USART2);	
}