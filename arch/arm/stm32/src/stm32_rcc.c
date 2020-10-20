#include <asm/stm32_rcc.h>
#include <igris/util/bug.h>

uint32_t stm32_clockbus_freq[6];

void stm32_rcc_enable_gpio(GPIO_TypeDef * gpio)
{
	switch ((uintptr_t)gpio)
	{
#ifdef RCC_AHB1ENR_GPIOAEN
		case GPIOA_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; break;
#endif

#ifdef RCC_AHB2ENR_GPIOAEN
		case GPIOA_BASE : RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; break;
#endif

#ifdef RCC_AHB1ENR_GPIOBEN
		case GPIOB_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; break;
#endif

#ifdef RCC_AHB2ENR_GPIOBEN
		case GPIOB_BASE : RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; break;
#endif

#ifdef RCC_AHB1ENR_GPIOCEN
		case GPIOC_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; break;
#endif

#ifdef RCC_AHB2ENR_GPIOCEN
		case GPIOC_BASE : RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; break;
#endif

#ifdef RCC_AHB1ENR_GPIODEN
		case GPIOD_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; break;
#endif

#ifdef RCC_AHB2ENR_GPIODEN
		case GPIOD_BASE : RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN; break;
#endif

#ifdef RCC_AHB1ENR_GPIOEEN
		case GPIOE_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; break;
#endif

#ifdef RCC_AHB2ENR_GPIOFEN
		case GPIOF_BASE : RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN; break;
#endif

#ifdef RCC_AHB1ENR_GPIOFEN
		case GPIOF_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; break;
#endif

#ifdef RCC_AHB1ENR_GPIOGEN
		case GPIOG_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; break;
#endif

#ifdef RCC_AHB1ENR_GPIOHEN
		case GPIOH_BASE : RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; break;
#endif

#ifdef RCC_AHB2ENR_GPIOHEN
		case GPIOH_BASE : RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN; break;
#endif

		default: BUG();
	}
}

void stm32_rcc_enable_usart(USART_TypeDef * usart)
{
	switch ((uintptr_t)usart)
	{
#ifdef RCC_APB1ENR_USART2EN
		case USART2_BASE : RCC->APB1ENR |= RCC_APB1ENR_USART2EN; break;
#elif defined(RCC_APB1ENR1_USART2EN)
		case USART2_BASE : RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; break;
#endif

#ifdef RCC_APB2ENR_USART6EN
		case USART6_BASE : RCC->APB2ENR |= RCC_APB2ENR_USART6EN; break;
#endif

#ifdef RCC_APB1ENR2_LPUART1EN
		case LPUART1_BASE : RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN; break;
#endif

#ifdef RCC_APB2ENR_USART1EN
		case USART1_BASE : RCC->APB2ENR |= RCC_APB2ENR_USART1EN; break;
#endif

		default: BUG();
	}
}

void stm32_rcc_enable_timer(TIM_TypeDef* t)
{
	switch ((uintptr_t)t)
	{
		case TIM1_BASE:
			RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; break;


#ifdef RCC_APB1ENR_TIM2EN
		case TIM2_BASE: RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; break;
#endif

#ifdef RCC_APB1ENR_TIM3EN
		case TIM3_BASE: RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; break;
#endif

#ifdef RCC_APB1ENR_TIM4EN
		case TIM4_BASE:	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; break;
#endif

#ifdef RCC_APB1ENR1_TIM4EN
		case TIM4_BASE:	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN; break;
#endif

#ifdef RCC_APB1ENR_TIM5EN
		case TIM5_BASE: RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; break;
#endif

#ifdef TIM6_BASE
#ifdef RCC_APB1ENR_TIM6EN
		case TIM6_BASE: RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; break;
#endif
#endif

#ifdef TIM7_BASE
#ifdef RCC_APB1ENR_TIM7EN
		case TIM7_BASE: RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; break;
#endif
#endif

#ifdef TIM8_BASE
		case TIM8_BASE:
			RCC->APB2ENR |= RCC_APB2ENR_TIM8EN; break;
#endif

#ifdef TIM9_BASE
		case TIM9_BASE:
			RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; break;
#endif

#ifdef TIM10_BASE
		case TIM10_BASE:
			RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; break;
#endif

#ifdef TIM11_BASE
		case TIM11_BASE: RCC->APB2ENR |= RCC_APB2ENR_TIM11EN; break;
#endif

#ifdef TIM12_BASE
		case TIM12_BASE:
			RCC->APB1ENR |= RCC_APB1ENR_TIM12EN; break;
#endif

#ifdef TIM13_BASE
		case TIM13_BASE:
			RCC->APB1ENR |= RCC_APB1ENR_TIM13EN; break;
#endif

#ifdef RCC_APB1ENR_TIM14EN
		case TIM14_BASE:
			RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; break;
#endif

		default: BUG();
	}
}

void stm32_rcc_enable_adc(ADC_TypeDef* regs)
{
	switch ((uintptr_t)regs)
	{
#ifdef RCC_APB2ENR_ADC1EN
		case ADC1_BASE :
			RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; break;
#endif

#ifdef RCC_APB2ENR_ADC2EN
		case ADC2_BASE :
			RCC->APB2ENR |= RCC_APB2ENR_ADC2EN; break;
#endif

#ifdef RCC_APB2ENR_ADC3EN
		case ADC3_BASE :
			RCC->APB2ENR |= RCC_APB2ENR_ADC3EN; break;
#endif

		default: BUG();
	}
}

void stm32_rcc_enable_i2c(I2C_TypeDef* regs)
{
	switch ((uintptr_t)regs)
	{
#ifdef RCC_APB1ENR_I2C1EN
		case I2C1_BASE :
			RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; break;
#endif

		default: BUG();
	}
}

void stm32_rcc_enable_spi(SPI_TypeDef* regs)
{
	switch ((uintptr_t)regs)
	{
#ifdef RCC_APB2ENR_SPI1EN
		case SPI1_BASE :
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; break;
#endif

#ifdef RCC_APB1ENR_SPI3EN
		case SPI3_BASE :
			RCC->APB1ENR |= RCC_APB1ENR_SPI3EN; break;
#endif

#ifdef RCC_APB2ENR_SPI4EN
		case SPI4_BASE :
			RCC->APB2ENR |= RCC_APB2ENR_SPI4EN; break;
#endif

		default: BUG();
	}
}

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