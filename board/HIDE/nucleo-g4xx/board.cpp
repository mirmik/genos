#include <hal/board.h>
#include <hal/arch.h>
#include "curboard.h"

#include <periph/map.h>

#include <asm/stm32_rcc.h>
#include <asm/stm32_gpio.h>

#include <util/cpu_delay.h>

namespace board 
{
	gpio_pin sysled 			( SYSLED_GPIO, SYSLED_MASK );
	genos::stm32_usart sysuart 	( SYSUART, SYSUART_IRQ );
}

void board_init(int freqmode) 
{
	arch_init();

	stm32_rcc_enable_gpio(SYSLED_GPIO);
	stm32_gpio_set_output(SYSLED_GPIO, SYSLED_MASK );
	stm32_gpio_set_level(SYSLED_GPIO, SYSLED_MASK , 1);

	/*stm32_gpio_set_output(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN));
	while(1) {
		cpu_delay(500);
		stm32_gpio_set_level(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), 0);
		cpu_delay(500);
		stm32_gpio_set_level(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), 1);
	}*/

/*	stm32_rcc_enable_usart(SYSUART);
	stm32_rcc_enable_gpio(SYSUART_RX_GPIO);
	stm32_rcc_enable_gpio(SYSUART_TX_GPIO);
	gpio_settings(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), GPIO_MODE_ALTERNATE);
	gpio_settings(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), GPIO_MODE_ALTERNATE);
	gpio_alternate(SYSUART_RX_GPIO, (1 << SYSUART_RX_PIN), SYSUART_RX_AF);
	gpio_alternate(SYSUART_TX_GPIO, (1 << SYSUART_TX_PIN), SYSUART_TX_AF);

	board::sysuart.setup(9600, 'n', 8, 1);

	stm32_gpio_set_level(SYSLED_GPIO, SYSLED_MASK , 0);*/
	//while(1) {
	//	board::sysled.toggle();
	//	cpu_delay(500000);
	//	SYSUART->TDR = 's';
	//}
}
