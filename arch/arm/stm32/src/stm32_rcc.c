#include <periph/map.h>
#include <periph/regs/gpio.h>
#include <periph/regs/usart.h>
#include <igris/util/bug.h>

uint32_t stm32_declared_clockbus_freq[3];

void rcc_reset()
{
	RCC->CR |= 		(uint32_t)0x00000001;
	RCC->CFGR = 	(uint32_t)0x00000000;
	RCC->CR &= 		(uint32_t)0xFEF6FFFF;
	RCC->PLLCFGR = 	(uint32_t)0x24003010;
	RCC->CR &= 		(uint32_t)0xFFFBFFFF;
	RCC->CIR = 		(uint32_t)0x00000000;
};

void rcc_enable_gpio(struct gpio_regs* g)
{
	switch ((uintptr_t)g)
	{
		case GPIOA_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; break;

		case GPIOB_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; break;

		case GPIOC_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; break;

		case GPIOD_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; break;

		case GPIOE_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; break;

		case GPIOF_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; break;

		case GPIOG_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; break;

		case GPIOH_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; break;

		default: BUG();
	}
}

#ifdef CHIP_STM32F1XX
void rcc_enable_afio(struct gpio_regs* g)
{
	switch ((uintptr_t)g)
	{
		case GPIOA_BASE :
		case GPIOD_BASE :
			RCC->AHB1ENR |= RCC_AHB1ENR_AFIOEN;
			break;

		default: BUG();
	}
}
#endif

void rcc_enable_usart(struct usart_regs* u)
{
	switch ((uintptr_t)u)
	{
		case USART2_BASE : RCC->APB1ENR |= RCC_APB1ENR_USART2EN; break;
		case USART6_BASE : RCC->APB2ENR |= RCC_APB2ENR_USART6EN; break;
		default: BUG();
	}
}