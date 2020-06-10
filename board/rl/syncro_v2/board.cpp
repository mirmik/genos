#include <hal/board.h>
#include "curboard.h"

#include <asm/stm32_clockbus.h>
#include <asm/stm32_rcc.h>
#include <asm/stm32_pll.h>
#include <asm/stm32_usart.h>
#include <asm/stm32_gpio.h>
#include <asm/stm32_diag.h>
#include <asm/stm32_systick.h>

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <util/cpu_delay.h>

#include <periph/map.h>

genos::stm32_usart usart2(USART2, USART2_IRQn);
genos::stm32_usart usart6(USART6, USART6_IRQn);

GPIO_PIN(board_led, GPIOD, 14);
GPIO_PIN(board_led2, GPIOD, 15);

struct gpio_pin extpin[4] = 
{
	{GPIOB, 1<<12},
	{GPIOB, 1<<13},
	{GPIOB, 1<<14},
	{GPIOB, 1<<15}
};

void usart6_configure()
{
	stm32_rcc_enable_gpio(GPIOC);
	stm32_rcc_enable_usart(USART6);

	cpu_delay(100);
	
	gpio_settings(GPIOC, (1 << 6 | 1 << 7), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_maxspeed(GPIOC, (1 << 6 | 1 << 7), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(GPIOC, (1 << 6 | 1 << 7), GPIO_AF_USART6);

	nvic_enable_irq(USART6_IRQn);

	stm32_usart_setup(USART6, 115200, 'n', 9, 1);
}

void board_init(int mode) 
{
	arch_init();
	
	struct stm32_pll_settings pll_settings = {
		.Mkoeff = 8,
		.Nkoeff = 336,
		.Pkoeff = 2,
		.Qkoeff = 7
	}; //84 000 000 Гц //168 000 000

	stm32_init_pll_clocking(&pll_settings);
	stm32_systick_config(168000);
	systime_set_frequency(1000);

	stm32_clockbus_freq[CLOCKBUS_PLL] = 168000000; 
	//stm32_declared_clockbus_freq[CLOCKBUS_NO_APB1] = 84000000;
	//stm32_declared_clockbus_freq[CLOCKBUS_NO_APB2] = 42000000;
	
	stm32_clockbus_freq[CLOCKBUS_APB1] = 42000000;
	stm32_clockbus_freq[CLOCKBUS_APB2] = 84000000;

	stm32_rcc_enable_usart(USART2);
	stm32_rcc_enable_usart(USART6);
	stm32_rcc_enable_gpio(GPIOA);
	stm32_rcc_enable_gpio(GPIOC);
	stm32_rcc_enable_gpio(GPIOD);

	cpu_delay(100);

	gpio_settings(GPIOA, (1 << 2 | 1 << 3), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_maxspeed(GPIOA, (1 << 2 | 1 << 3), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(GPIOA, (1 << 2 | 1 << 3), GPIO_AF_USART2);

	stm32_usart_setup(USART2, 115200, 'n', 8, 1);
	stm32_diag_init(USART2);	

	usart6_configure();

	//board::sysled0.settings(GPIO_MODE_OUTPUT);
	//board::sysled1.settings(GPIO_MODE_OUTPUT);

	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT);
	gpio_pin_settings(&board_led2, GPIO_MODE_OUTPUT);
}