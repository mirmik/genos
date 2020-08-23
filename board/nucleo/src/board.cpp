#include <hal/board.h>
#include "curboard.h"

#include <drivers/gpio/gpio.h>
#include <systime/systime.h>

#include <periph/map.h>
#include <util/cpu_delay.h>

#include <asm/stm32_usart.h>
#include <asm/stm32_gpio.h>
#include <asm/stm32_diag.h>
#include <asm/stm32_pll.h>
#include <asm/stm32_systick.h>
#include <asm/stm32_clockbus.h>

gpio_pin board::sysled(SYSLED_GPIO, SYSLED_MASK);
genos::stm32_usart board::sysuart(SYSUART, SYSUART_IRQ);
//stm32_usart_device usart6 (USART6, STM32_IRQ_USART6);


void board_init(int freqmode)
{
	arch_init();

	// sysled
	stm32_rcc_enable_gpio(SYSLED_GPIO);
	stm32_gpio_set_output(SYSLED_GPIO, SYSLED_MASK );
	stm32_gpio_set_level(SYSLED_GPIO, SYSLED_MASK, 1);

	// freq
	stm32_clockbus_freq[CLOCKBUS_HSE] = BOARD_HSE_FREQ;
	switch (freqmode)
	{
		case STM32_FREQMODE_HSI: //HSI mode
			stm32_clockbus_hsi_mode();
			break;

		case STM32_FREQMODE_HSE: 
			stm32_clockbus_hse_mode();
			break;
	}
	stm32_clockbus_reevaluate();
	stm32_clockbus_systime_setup();

	/// sysuart
	stm32_rcc_enable_gpio(SYSUART_RX_GPIO);
	stm32_rcc_enable_gpio(SYSUART_TX_GPIO);
	stm32_rcc_enable_usart(SYSUART);

	gpio_settings(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), GPIO_MODE_ALTERNATE);
	//stm32_gpio_set_maxspeed(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), SYSUART_RX_AF);
	
	gpio_settings(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), GPIO_MODE_ALTERNATE);
	//stm32_gpio_set_maxspeed(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), SYSUART_TX_AF);

	board::sysuart.setup(115200, 'n', 8, 1);
	stm32_diag_init(SYSUART);
}

/*void board_init(int freqmode)
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

	stm32_rcc_enable_gpio(BOARD_LED_GPIO);
	stm32_rcc_enable_gpio(SYSUART_RX_GPIO);
	stm32_rcc_enable_gpio(SYSUART_TX_GPIO);
	stm32_rcc_enable_usart(SYSUART);

	cpu_delay(100);

	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);

	gpio_settings(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), GPIO_MODE_ALTERNATE);
	//stm32_gpio_set_maxspeed(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), SYSUART_AF);
	
	gpio_settings(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), GPIO_MODE_ALTERNATE);
	//stm32_gpio_set_maxspeed(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), STM32_GPIO_SPEED_LEVEL_0);
	stm32_gpio_set_alternate(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), SYSUART_AF);

	stm32_usart_setup(SYSUART, 115200, 'n', 8, 1);
	stm32_diag_init(SYSUART);
}*/