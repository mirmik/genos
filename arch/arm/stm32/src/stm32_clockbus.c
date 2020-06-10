#include <asm/stm32_clockbus.h>
#include <asm/stm32_systick.h>
#include <periph/map.h>
#include <systime/systime.h>

static int __active_mode = 0;
static int __systembus = 0;

int __dividers[2] = {1, 1};

void stm32_clockbus_hsi_mode() 
{
	//volatile int i;
	//RCC->CR = RCC_CR_HSION;
	//i = 1000; while (--i);
	__active_mode = 0;
	__systembus = CLOCKBUS_HSI;
	stm32_clockbus_reevaluate();
}

void stm32_clockbus_set_dividers(int a, int b) 
{
	__dividers[0] = a;
	__dividers[1] = b;
}

void stm32_clockbus_hse_mode() 
{
	volatile int i;
	RCC->CR |= RCC_CR_CSSON;
	i = 1000; while (--i);

#ifdef RCC_APB1ENR1_PWREN
	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;              // Power interface clock enable
#else
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
#endif

	i = 1000; while (--i);

#ifdef RCC_APB1ENR1_PWREN
	while ((RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) != RCC_APB1ENR1_PWREN) {};
#else
	while ((RCC->APB1ENR & RCC_APB1ENR_PWREN) != RCC_APB1ENR_PWREN) {};
#endif

	RCC->CFGR &= ~(RCC_CFGR_HPRE_DIV16 | RCC_CFGR_PPRE1_DIV16 | RCC_CFGR_PPRE2_DIV16);
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 
	             | RCC_CFGR_PPRE1_DIV1
	             | RCC_CFGR_PPRE2_DIV1;
	stm32_clockbus_set_dividers(1, 1);

	// Запуск HSE и ожидание включения
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (RCC_CFGR_SW_HSE);
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);

	//RCC->CR &= ~RCC_CR_HSION;
	//i = 1000; while (--i);
	
	__active_mode = 1;
	__systembus = CLOCKBUS_HSE;
	stm32_clockbus_reevaluate();
}

void stm32_clockbus_reevaluate() 
{	
	switch (__active_mode) 
	{ 	
		case 0:
			stm32_clockbus_freq[CLOCKBUS_APB1] = stm32_clockbus_freq[CLOCKBUS_HSI] / __dividers[0];
			stm32_clockbus_freq[CLOCKBUS_APB2] = stm32_clockbus_freq[CLOCKBUS_HSI] / __dividers[1];
			break;
		case 1:
			stm32_clockbus_freq[CLOCKBUS_APB1] = stm32_clockbus_freq[CLOCKBUS_HSE] / __dividers[0];
			stm32_clockbus_freq[CLOCKBUS_APB2] = stm32_clockbus_freq[CLOCKBUS_HSE] / __dividers[1];
			break;
		case 2:
			stm32_clockbus_freq[CLOCKBUS_APB1] = stm32_clockbus_freq[CLOCKBUS_PLL] / __dividers[0];
			stm32_clockbus_freq[CLOCKBUS_APB2] = stm32_clockbus_freq[CLOCKBUS_PLL] / __dividers[1];
			break;
	}
}

void stm32_clockbus_systime_setup() 
{
	stm32_systick_config(stm32_clockbus_freq[__systembus] / 1000);
	sysclock_set_frequency(stm32_clockbus_freq[__systembus]);
	init_delays(stm32_clockbus_freq[__systembus]);
	systime_set_frequency(1000);
}

int stm32_clockbus_enable_pll_mode(uint32_t freq) 
{
	__active_mode = 2;
	__systembus = CLOCKBUS_PLL;
	stm32_clockbus_freq[__systembus] = freq;
	stm32_clockbus_reevaluate();
	stm32_clockbus_systime_setup();

	return 0;
}