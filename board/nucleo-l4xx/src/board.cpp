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
//stm32_usart_device debug_usart(DEBUG_USART, DEBUG_USART_IRQ);
//stm32_usart_device usart6 (USART6, STM32_IRQ_USART6);

void board_init(int freqmode)
{
	arch_init();

	switch (freqmode)
	{
/*		case 0:
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
*/
		case 2:
			stm32_declared_clockbus_freq[CLOCKBUS_NO_PLL] = 16000000;
			stm32_declared_clockbus_freq[CLOCKBUS_NO_APB1] = 16000000;
			stm32_declared_clockbus_freq[CLOCKBUS_NO_APB2] = 16000000;
			break;
	};

/*	systime_set_frequency(1000);
*/
	stm32l4_rcc_enable_gpio(BOARD_LED_GPIO);
	stm32l4_rcc_enable_gpio(GPIOA);
	stm32l4_rcc_enable_usart(LPUART1);

	bits_assign(RCC->CCIPR, RCC_CCIPR_LPUART1SEL_Msk, (0b01) << RCC_CCIPR_LPUART1SEL_Pos);

	cpu_delay(100);

	gpio_pin_settings(&board_led, GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);
	
	gpio_settings(GPIOA, (1 << 2), GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);
	gpio_settings(GPIOA, (1 << 2) | (1 << 3), GPIO_MODE_ALTERNATE);
	//stm32_gpio_set_maxspeed(DEBUG_USART_RX_GPIO, (1 << DEBUG_USART_RX_PIN), STM32_GPIO_SPEED_LEVEL_0);
	stm32l4_gpio_set_alternate(GPIOA, (1 << 2) | (1 << 3), 8);
	
	//stm32l4_usart_setup(LPUART1, 115200, 'n', 8, 1);
	stm32l4_usart_setup(LPUART1, 115200, 'n', 8, 1);
	//LPUART1->GTPR = 1;

	//LPUART1->BRR = 0x0F000034;
	//int32_t BAUDRATE = 9600;

	//LPUART1->BRR = 16000000 / 152000;
	//LPUART1->BRR = 0x2A55;
	//LPUART1->BRR = 0x2555;
	//LPUART1->BRR = 35552;


	stm32_usart_putc(LPUART1, 'A');
	//stm32_usart_putc(DEBUG_USART, 'A');
	stm32l4_diag_init(LPUART1);

	/*while(1) 
	{
		//board::sysled.toggle();
		//delay_us(5000);
		cpu_delay(100000);
	//	stm32_usart_putc(LPUART1, 'A');
	
		board::sysled.toggle();
	}*/

}