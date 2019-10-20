#include <asm/rcc.h>
#include <igris/util/bug.h>

void stm32l4_rcc_reset()
{
	RCC->CR |= 		(uint32_t)0x00000001;
	RCC->CFGR = 	(uint32_t)0x00000000;
	RCC->CR &= 		(uint32_t)0xFEF6FFFF;
	RCC->PLLCFGR = 	(uint32_t)0x24003010;
	RCC->CR &= 		(uint32_t)0xFFFBFFFF;
	RCC->CIER = 	(uint32_t)0x00000000;
}

void stm32l4_rcc_enable_gpio(GPIO_TypeDef * gpio)
{
	switch ((uintptr_t)gpio)
	{
		case GPIOA_BASE :
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; break;

		case GPIOB_BASE :
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; break;

		case GPIOC_BASE :
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; break;

#ifdef GPIOD
		case GPIOD_BASE :
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN; break;
#endif

		case GPIOH_BASE :
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN; break;

		default: BUG();
	}
}

void stm32l4_rcc_enable_usart(USART_TypeDef * usart)
{
	switch ((uintptr_t)usart)
	{
		case LPUART1_BASE :
			RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN; break;

		case USART1_BASE :
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN; break;

		case USART2_BASE :
			RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; break;

#ifdef USART3
		case USART3_BASE :
			RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN; break;
#endif

		default: BUG();
	}
}


void stm32l4_rcc_enable_spi(SPI_TypeDef * spi) 
{
	switch ((uintptr_t)spi)
	{
		case SPI1_BASE :
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; break;

		//case SPI2_BASE :
		//	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; break;
//
		//case SPI3_BASE :
		//	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN; break;
		//
		//case SPI4_BASE :
		//	RCC->APB2ENR |= RCC_APB2ENR_SPI4EN; break;
		//
		//case SPI5_BASE :
		//	RCC->APB2ENR |= RCC_APB2ENR_SPI5EN; break;
		//
		//case SPI6_BASE :
		//	RCC->APB2ENR |= RCC_APB2ENR_SPI6EN; break;
		
		default: BUG();
	}
}