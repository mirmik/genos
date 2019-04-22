#ifndef RL_SYNCRO_V2_BOARD
#define RL_SYNCRO_V2_BOARD

#include <sys/cdefs.h>
#include <drivers/serial/stm32_usart.h>
#include <drivers/gpio/xpin.h>

#define HSE_FREQ 8000000

extern struct stm32_usart_device usart2;
extern struct stm32_usart_device usart6;

namespace board 
{
	extern genos::drivers::xpin sysled0;
	extern genos::drivers::xpin sysled1;
}

__BEGIN_DECLS

void board_init();

__END_DECLS

#endif