#include <zillot/stm32/stm32_systick.h>
#include <zillot/armv7e-m/arm_nvic.h>

uint32_t __systick_config = 0;

int stm32_systick_config(uint32_t ticks)
{
	if ((ticks - 1) > SYSTICK_VALUE_MASK)
		return 1; // reload value impossible

	__systick_config = ticks;

	SysTick->LOAD = ticks - 1; // set reload register
	SysTick->VAL = 0; // reset current value

	// set max priority for systick interrupt
	nvic_set_priority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

	// enable systick irq and systick timer
	SysTick->CTRL  = SYSTICK_CTRL_CLKSOURCE |
	                SYSTICK_CTRL_TICKINT |
	                SYSTICK_CTRL_ENABLE;

	// Success
	return 0;
}