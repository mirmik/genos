#include <drivers/stm32/rcc.h>
#include <debug/dprint.h>

void stm32_sysclk_hse_init() {
	RCC->CR |= RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY) == 0);
}