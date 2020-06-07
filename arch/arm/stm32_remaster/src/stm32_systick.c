#include <asm/stm32_systick.h>
#include <periph/coremap.h>
#include <periph/map.h>

#include <asm/nvic.h>

int stm32_systick_config(uint32_t ticks)
{
	if ((ticks - 1) > SYSTICK_VALUE_MASK)
		return 1; // reload value impossible

	SysTick->LOAD = ticks - 1; // set reload register
	SysTick->VAL = 0; // reset current value

	// set max priority for systick interrupt
	//nvic_set_priority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

	// enable systick irq and systick timer
	SysTick->CTRL  = SYSTICK_CTRL_CLKSOURCE |
	                SYSTICK_CTRL_TICKINT |
	                SYSTICK_CTRL_ENABLE;

	// Success
	return 0;
}