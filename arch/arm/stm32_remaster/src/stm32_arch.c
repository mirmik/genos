#include <hal/arch.h>

#include <periph/map.h>
#include <asm/stm32_rcc.h>

#include <asm/irq.h>

void arch_init() 
{
	volatile int i;

	irqs_disable();

	stm32_rcc_reset();
	i = 1000; while (--i);

	RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->AHB2RSTR = 0xFFFFFFFF;
	RCC->AHB3RSTR = 0xFFFFFFFF;
	RCC->APB1RSTR1 = 0xFFFFFFFF;
	RCC->APB1RSTR2 = 0xFFFFFFFF;
	RCC->APB2RSTR = 0xFFFFFFFF;
	i = 1000; while (--i);

	RCC->AHB1RSTR = 0x00000000;
	RCC->AHB2RSTR = 0x00000000;
	RCC->AHB3RSTR = 0x00000000;
	RCC->APB1RSTR1 = 0x00000000;
	RCC->APB1RSTR2 = 0x00000000;
	RCC->APB2RSTR = 0x00000000;
	i = 1000; while (--i);


}