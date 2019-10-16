#include <hal/board.h>
#include "curboard.h"

#include <asm/arch.h>
#include <asm/rcc.h>
#include <asm/usart.h>
#include <asm/gpio.h>
#include <asm/delay.h>

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <periph/map.h>
#include <util/cpu_delay.h>

GPIO_PIN(board_led, BOARD_LED_GPIO, BOARD_LED_PIN);
stm32_usart_device debug_usart(DEBUG_USART, DEBUG_USART_IRQ);
//stm32_usart_device usart6 (USART6, STM32_IRQ_USART6);

void board_init(int freqmode)
{
	arch_init();

	switch (freqmode)
	{
		case 0:
		{
			struct stm32_pll_settings pll_settings =
			{
				.Mkoeff = 8,
				.Nkoeff = 336,
				.Pkoeff = 4,
				.Qkoeff = 7
			}; //84 000 000 Гц


			stm32_init_pll_clocking(&pll_settings);
			stm32_systick_config(84000);
			sysclock_set_frequency(84000000);
			init_delays(84000000);

			stm32_declared_clockbus_freq[CLOCKBUS_NO_PLL] = 84000000;
			stm32_declared_clockbus_freq[CLOCKBUS_NO_APB1] = 42000000;
			stm32_declared_clockbus_freq[CLOCKBUS_NO_APB2] = 84000000;
		}
		break;

		case 1:
		{
			struct stm32_pll_settings pll_settings =
			{
				.Mkoeff = 8,
				.Nkoeff = 64,
				.Pkoeff = 4,
				.Qkoeff = 7
			}; //16 000 000 Гц


			stm32_init_pll_clocking(&pll_settings);
			stm32_systick_config(16000);
			sysclock_set_frequency(16000000);
			init_delays(16000000);

			stm32_declared_clockbus_freq[CLOCKBUS_NO_PLL] = 16000000;
			stm32_declared_clockbus_freq[CLOCKBUS_NO_APB1] = 8000000;
			stm32_declared_clockbus_freq[CLOCKBUS_NO_APB2] = 16000000;
		}
		break;

		case 2:
			break;

	};

	systime_set_frequency(1000);

	rcc_enable_gpio(BOARD_LED_GPIO);
	rcc_enable_usart(DEBUG_USART);

	cpu_delay(100);

	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);

	gpio_settings(DEBUG_USART_RX_GPIO, (1 << DEBUG_USART_RX_PIN), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_maxspeed(DEBUG_USART_RX_GPIO, (1 << DEBUG_USART_RX_PIN), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(DEBUG_USART_RX_GPIO, (1 << DEBUG_USART_RX_PIN), DEBUG_USART_AF);
	
	gpio_settings(DEBUG_USART_TX_GPIO, (1 << DEBUG_USART_TX_PIN), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_maxspeed(DEBUG_USART_TX_GPIO, (1 << DEBUG_USART_TX_PIN), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(DEBUG_USART_TX_GPIO, (1 << DEBUG_USART_TX_PIN), DEBUG_USART_AF);

	stm32_usart_setup(DEBUG_USART, 115200, 'n', 8, 1);
	stm32_diag_init(DEBUG_USART);
}