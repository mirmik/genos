#ifndef NUCLEO_BOARDS_DEFS_H
#define NUCLEO_BOARDS_DEFS_H

#define STM32_FREQMODE_HSI 0
#define STM32_FREQMODE_HSE 1
#define STM32_FREQMODE_PLL_HIGH 2
#define STM32_FREQMODE_PLL_LOW 3

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>
#include <drivers/serial/stm32_usart.h>

namespace board 
{
	extern gpio_pin sysled;
	extern genos::stm32_usart sysuart;
	extern genos::stm32_usart arduart;
}

__BEGIN_DECLS

void board_init(int freqmode = STM32_FREQMODE_HSI);

__END_DECLS

#endif