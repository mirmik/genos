#include <periph/map.h>
#include <periph/regs/gpio.h>
#include <periph/regs/usart.h>
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
		case (uintptr_t)GPIOA : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; break;
		default: panic("rcc_enable_gpio");
	}
}

void rcc_enable_usart(struct usart_regs* u) {
	switch ((uintptr_t)u) {
		case (uintptr_t)USART2 : RCC->APB1ENR |= RCC_APB1ENR_USART2EN; break;
		default: panic("rcc_enable_usart");
	}
}