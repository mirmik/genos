#include <asm/stm32_rcc.h>
#include <igris/util/bug.h>

void stm32_rcc_enable_gpio(GPIO_TypeDef * gpio)
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

#ifdef GPIOH
		case GPIOH_BASE :
			RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN; break;
#endif

		default: BUG();
	}
}

void stm32_rcc_enable_usart(USART_TypeDef * usart);

void stm32_rcc_enable_spi(SPI_TypeDef * spi);

void stm32_rcc_reset()
{
	/*RCC->CR |= 		(uint32_t)0x00000001;
	RCC->CFGR = 	(uint32_t)0x00000000;
	RCC->CR &= 		(uint32_t)0xFEF6FFFF;
	RCC->PLLCFGR = 	(uint32_t)0x24003010;
	RCC->CR &= 		(uint32_t)0xFFFBFFFF;
	RCC->CIER = 	(uint32_t)0x00000000;*/

	/* FPU settings ------------------------------------------------------------*/
//#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
//	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
//#endif
	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set MSION bit */
//	RCC->CR |= RCC_CR_MSION;

	/* Reset CFGR register */
//	RCC->CFGR = 0x00000000;

	/* Reset HSEON, CSSON , HSION, and PLLON bits */
//	RCC->CR &= (uint32_t)0xEAF6FFFF;

	/* Reset PLLCFGR register */
//	RCC->PLLCFGR = 0x00001000;

	/* Reset HSEBYP bit */
//	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Disable all interrupts */
//	RCC->CIER = 0x00000000;
}