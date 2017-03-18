#include <periph/map.h>
#include <kernel/panic.h>

void rcc_reset() {
	RCC->CR |= 		(uint32_t)0x00000001;
	RCC->CFGR = 	(uint32_t)0x00000000;
	RCC->CR &= 		(uint32_t)0xFEF6FFFF;
	RCC->PLLCFGR = 	(uint32_t)0x24003010;
	RCC->CR &= 		(uint32_t)0xFFFBFFFF;
	RCC->CIR = 		(uint32_t)0x00000000;
};

void rcc_enable_gpio(struct gpio_regs* g) {
	switch ((uintptr_t)g) {
		case (uintptr_t)GPIOD : RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; break;
		default: panic("rcc_enable_gpio");
	}
}