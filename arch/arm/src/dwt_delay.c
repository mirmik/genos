#include <stdint.h>

void stm32_dwt_init() 
{
	DWT->CTRL |= 1 ; // enable the counter
	DWT->CYCCNT = 0; // reset the counter
}

#pragma GCC push_options
#pragma GCC optimize ("O3")
void delayMilliseconds(uint32_t us) {
	volatile uint32_t cycles = (SystemCoreClock/1000000L)*us;
	volatile uint32_t start = DWT->CYCCNT;
	do  {
	} while(DWT->CYCCNT - start < cycles);
}
#pragma GCC pop_options
