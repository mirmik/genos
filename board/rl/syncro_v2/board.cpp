#include <hal/board.h>
#include "curboard.h"

#include <asm/arch.h>
#include <asm/rcc.h>
#include <asm/usart.h>
#include <asm/gpio.h>

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <periph/map.h>

STM32_USART_DEVICE_DECLARE(usart2, USART2, STM32_IRQ_USART2);
STM32_USART_DEVICE_DECLARE(usart6, USART6, STM32_IRQ_USART6);

struct gpio_pin board_led;

genos::drivers::xpin board::sysled0(GPIOD, 14);
genos::drivers::xpin board::sysled1(GPIOD, 15);

struct gpio_pin extpin[4] = 
{
	{GPIOC, 1<<10},
	{GPIOC, 1<<11},
	{GPIOC, 1<<12},
	{GPIOA, 1<<15}
};

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
	rcc_enable_usart(USART6);
	rcc_enable_gpio(GPIOA);
	rcc_enable_gpio(GPIOD);

	gpio_settings(GPIOA, (1 << 2 | 1 << 3), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_maxspeed(GPIOA, (1 << 2 | 1 << 3), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(GPIOA, (1 << 2 | 1 << 3), 7);

	stm32_usart_setup(USART2, 115200, 'n', 8, 1);
	stm32_diag_init(USART2);	

	board::sysled0.settings(GPIO_MODE_OUTPUT);
	board::sysled1.settings(GPIO_MODE_OUTPUT);
}