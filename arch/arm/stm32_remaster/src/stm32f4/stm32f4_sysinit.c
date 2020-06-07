#include <periph/map.h>
#include <periph/coremap.h>

void stm32_sysinit(void)
{
	volatile int i;

	stm32_rcc_reset();

	RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->APB1RSTR = 0xFFFFFFFF;
	RCC->APB2RSTR = 0xFFFFFFFF;
	i = 1000; while (--i);

	RCC->AHB1RSTR = 0;
	RCC->APB1RSTR = 0;
	RCC->APB2RSTR = 0;
	i = 1000; while (--i);
}
