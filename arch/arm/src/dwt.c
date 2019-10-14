#ifndef ARM_DWT_H
#define ARM_DWT_H

#include <periph/regs/dwt.h>
#include <periph/regs/coredebug.h>
#include <periph/map.h>
#include <systime/systime.h>

#include <igris/dprint.h>

void arm_dwt_init() 
{	
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= 1 ; // enable the counter
	DWT->CYCCNT = 0; // reset the counter
}

#pragma GCC push_options
#pragma GCC optimize ("O3")
void arm_dwt_delay_us(uint32_t us) {
	volatile uint32_t cycles = (sysclock_frequency()/1000000L)*us;
	volatile uint32_t start = DWT->CYCCNT;

	do  {
	} while(DWT->CYCCNT - start < cycles);
}
#pragma GCC pop_options


#endif