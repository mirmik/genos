#include <hal/board.h>
#include "curboard.h"

#include <asm/arch.h>
#include <asm/rcc.h>
#include <asm/usart.h>
#include <asm/gpio.h>

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <periph/map.h>

GPIO_PIN(board_led, GPIOA, 5);

void board_init() 
{
	arch_init();
	
	struct stm32_pll_settings pll_settings = {
		.Mkoeff = 8,
		.Nkoeff = 168,
		.Pkoeff = 2,
		.Qkoeff = 7
	}; //84 000 000 Гц

	stm32_init_pll_clocking(&pll_settings);
	stm32_systick_config(84000);
	systime_set_frequency(1000);

	stm32_declared_clockbus_freq[CLOCKBUS_NO_PLL] = 84000000; 
	stm32_declared_clockbus_freq[CLOCKBUS_NO_APB1] = 84000000;
	stm32_declared_clockbus_freq[CLOCKBUS_NO_APB2] = 42000000;

	rcc_enable_usart(USART2);
//	rcc_enable_usart(USART6);
	rcc_enable_gpio(GPIOA);
//	rcc_enable_gpio(GPIOD);

	gpio_settings(GPIOD, (0b1111 << 12), GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);

	gpio_settings(GPIOA, (1 << 2 | 1 << 3), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_maxspeed(GPIOA, (1 << 2 | 1 << 3), STM32_GPIO_2MHZ);
	stm32_gpio_set_alternate(GPIOA, (1 << 2 | 1 << 3), 7);

	stm32_usart_setup(USART2, 115200, 'n', 8, 1);
	stm32_diag_init(USART2);	
}