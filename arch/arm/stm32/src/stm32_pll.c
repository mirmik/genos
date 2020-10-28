#include <asm/stm32_pll.h>
#include <asm/stm32_clockbus.h>
#include <periph/map.h>

#include <igris/util/bits.h>

#if defined(STM32F4xx) || defined(CHIP_STM32F4XX) 
unsigned stm32_init_pll_clocking(struct stm32_pll_settings* s)
{
	// Включаем масштабирование регулятора напряжения.
	// Необходимо для высоких частот ???
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	/// Настраиваем задержки FLASH для работы при повышенной частоте.
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	// Настраиваем делители в цепочках тактирования
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 //RCC_CFGR_HPRE_DIV1
	             | RCC_CFGR_PPRE1_DIV4
	             | RCC_CFGR_PPRE2_DIV2;
	stm32_clockbus_set_dividers(4, 2);

	// Запуск HSE и ожидание включения
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Конфигурируем PLL, выбирая в качестве источника внешний кварц.
	RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE |
	                (RCC_PLLCFGR_PLLM & (s->Mkoeff << 0))
	                | (RCC_PLLCFGR_PLLN & (s->Nkoeff << 6)) 		// 50 >= PLLN >= 432
	                | (RCC_PLLCFGR_PLLP & (((s->Pkoeff >> 1) - 1) << 16))
	                | (RCC_PLLCFGR_PLLQ & (s->Qkoeff << 24))
	               );

	// Активируем PLL, ждём, пока он запустится.
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Тактируем шину от PLL, ждём подтверждения.
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (RCC_CFGR_SW_PLL);
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

	return 0;
}
#elif defined(STM32G4XX) || defined(STM32L4XX)

unsigned stm32_init_pll_clocking(struct stm32_pll_settings* s) 
{
	volatile int i;
	// Включаем масштабирование регулятора напряжения.
	// Необходимо для высоких частот ???
//	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
//	RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;
//	bits_assign(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_0);

	/// Настраиваем задержки FLASH для работы при повышенной частоте.
	//FLASH->ACR = //FLASH_ACR_ICEN; //| FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;
	//FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_4WS;
	//i = 1000; while (--i);

	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;              // Power interface clock enable
	i = 1000; while (--i);
	while ((RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) != RCC_APB1ENR1_PWREN)
	{
	};
 
	//PWR->CR1 |= PWR_CR1_VOS;                        // Voltage scaling = range 1
	bits_assign(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_0);
	i = 1000; while (--i);
	while ((PWR->CR1 & PWR_CR1_VOS) != PWR_CR1_VOS_0)
	{
	};   // Waits until scaling is ready
 
	//FLASH->ACR |= (// FLASH_ACR_PRFTEN |               // Prefetch enable
	//		FLASH_ACR_LATENCY_1WS);                 // FLASH 4 wait states

	bits_assign(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);
	bits_assign(FLASH->ACR, FLASH_ACR_PRFTEN, FLASH_ACR_PRFTEN);
	bits_assign(FLASH->ACR, FLASH_ACR_ICEN, FLASH_ACR_ICEN);
	bits_assign(FLASH->ACR, FLASH_ACR_DCEN, FLASH_ACR_DCEN);

	
	// Настраиваем делители в цепочках тактирования
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 //RCC_CFGR_HPRE_DIV1
	             | RCC_CFGR_PPRE1_DIV1
	             | RCC_CFGR_PPRE2_DIV1;
	stm32_clockbus_set_dividers(1, 1);

	// Запуск HSE и ожидание включения
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

    if (s->Rkoeff != 2) while(1) {};
	if (s->Pkoeff != 2) while(1) {};
	if (s->Qkoeff != 2) while(1) {};

	// Конфигурируем PLL, выбирая в качестве источника внешний кварц.
	RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE |
	                  (RCC_PLLCFGR_PLLM & ((s->Mkoeff - 1) << RCC_PLLCFGR_PLLM_Pos))
	                | (RCC_PLLCFGR_PLLN & (s->Nkoeff << RCC_PLLCFGR_PLLN_Pos)) // 8 >= PLLN >= 127
	                | (RCC_PLLCFGR_PLLR & (0b00 << RCC_PLLCFGR_PLLR_Pos))
	                | (RCC_PLLCFGR_PLLP & (0b00 << RCC_PLLCFGR_PLLP_Pos))
	                | (RCC_PLLCFGR_PLLQ & (0b00 << RCC_PLLCFGR_PLLQ_Pos))
	               );
    
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

	// Активируем PLL, ждём, пока он запустится.
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Тактируем шину от PLL, ждём подтверждения.
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (RCC_CFGR_SW_PLL);
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

	return 0;
}
#else 
#error "undefined stm32_init_pll_clocking"
#endif

#if defined(STM32_PLL_VAR1)
int stm32_pll_setup(int M, int N, int R, int P, int Q) 
{
	struct stm32_pll_settings koeffs = {M,N,R,P,Q};
	return stm32_init_pll_clocking(&koeffs);
}
#elif defined(STM32_PLL_VAR2)
int stm32_pll_setup(int M, int N, int P, int Q) 
{
	struct stm32_pll_settings koeffs = {M,N,P,Q};
	return stm32_init_pll_clocking(&koeffs);
}
#else
#error "Undefined PLL koefficients"
#endif
/*unsigned stm32_init_pll_clocking(struct stm32_pll_settings* s)
{
	// Включаем масштабирование регулятора напряжения.
	// Необходимо для высоких частот ???
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	/// Настраиваем задержки FLASH для работы при повышенной частоте.
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	// Настраиваем делители в цепочках тактирования
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 //RCC_CFGR_HPRE_DIV1
	             | RCC_CFGR_PPRE1_DIV4
	             | RCC_CFGR_PPRE2_DIV2;

	// Запуск HSE и ожидание включения
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Конфигурируем PLL, выбирая в качестве источника внешний кварц.
	RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE |
	                (RCC_PLLCFGR_PLLM & (s->Mkoeff << 0))
	                | (RCC_PLLCFGR_PLLN & (s->Nkoeff << 6)) 		// 50 >= PLLN >= 432
	                | (RCC_PLLCFGR_PLLP & (((s->Pkoeff >> 1) - 1) << 16))
	                | (RCC_PLLCFGR_PLLQ & (s->Qkoeff << 24))
	               );

	// Активируем PLL, ждём, пока он запустится.
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Тактируем шину от PLL, ждём подтверждения.
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (RCC_CFGR_SW_PLL);
	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

	return 0;
}*/