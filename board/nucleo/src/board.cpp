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

	stm32_gpio_mode(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_alternate(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), SYSUART_RX_AF);
	
	stm32_gpio_mode(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), GPIO_MODE_ALTERNATE);
	stm32_gpio_set_alternate(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), SYSUART_TX_AF);

	stm32_usart_setup(SYSUART, 115200, 'n', 8, 1);
	stm32_diag_init(SYSUART);
}


void blink_test_cpudelay(int cpudelay) 
{
	while(1) 
	{
		stm32_gpio_set_level(SYSLED_GPIO, SYSLED_MASK, 1);
		cpu_delay(cpudelay);
	}	
}