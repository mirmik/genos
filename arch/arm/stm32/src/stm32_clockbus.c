#include <asm/stm32_clockbus.h>
#include <asm/stm32_systick.h>
#include <periph/map.h>
#include <systime/systime.h>

#include <igris/util/bug.h>
#include <igris/util/bits.h>

static int __active_mode = 0;
static int __systembus = 0;

//int __dividers[2] = {1, 1};

int16_t __hpre_divider = 1;
int16_t __ppre1_divider = 1;
int16_t __ppre2_divider = 1;

void stm32_clockbus_hsi_mode() 
{
	RCC->CFGR &= ~(RCC_CFGR_HPRE_DIV16 | RCC_CFGR_PPRE1_DIV16 | RCC_CFGR_PPRE2_DIV16);
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 
	             | RCC_CFGR_PPRE1_DIV1
	             | RCC_CFGR_PPRE2_DIV1;
	
	__hpre_divider = 1;
	__ppre1_divider = 1;	
	__ppre2_divider = 1;

	__active_mode = 0;
	__systembus = CLOCKBUS_HSI;
	stm32_clockbus_reevaluate();
}

void stm32_clockbus_set_hrpe_divider(int div) 
{
	uint32_t val;
	
	switch (div) 
	{
		case 1: val = RCC_CFGR_HPRE_DIV1; break;
		case 2: val = RCC_CFGR_HPRE_DIV2; break;
		case 4: val = RCC_CFGR_HPRE_DIV4; break;
		case 8: val = RCC_CFGR_HPRE_DIV8; break;
		case 16: val = RCC_CFGR_HPRE_DIV16; break;
		case 64: val = RCC_CFGR_HPRE_DIV64; break;
		case 128: val = RCC_CFGR_HPRE_DIV128; break;
		case 256: val = RCC_CFGR_HPRE_DIV256; break;
		case 512: val = RCC_CFGR_HPRE_DIV512; break;
		default: BUG();
	}

	__hpre_divider = div;
	bits_assign(RCC->CFGR, RCC_CFGR_HPRE, val);
}


void stm32_clockbus_set_ppre1_divider(int div) 
{
	uint32_t val;
	
	switch (div) 
	{
		case 1: val = RCC_CFGR_PPRE1_DIV1; break;
		case 2: val = RCC_CFGR_PPRE1_DIV2; break;
		case 4: val = RCC_CFGR_PPRE1_DIV4; break;
		case 8: val = RCC_CFGR_PPRE1_DIV8; break;
		case 16: val = RCC_CFGR_PPRE1_DIV16; break;
		default: BUG();
	}

	__ppre1_divider = div;
	bits_assign(RCC->CFGR, RCC_CFGR_PPRE1, val);
}


void stm32_clockbus_set_ppre2_divider(int div) 
{
	uint32_t val;
	
	switch (div) 
	{
		case 1: val = RCC_CFGR_PPRE2_DIV1; break;
		case 2: val = RCC_CFGR_PPRE2_DIV2; break;
		case 4: val = RCC_CFGR_PPRE2_DIV4; break;
		case 8: val = RCC_CFGR_PPRE2_DIV8; break;
		case 16: val = RCC_CFGR_PPRE2_DIV16; break;
		default: BUG();
	}

	__ppre2_divider = div;
	bits_assign(RCC->CFGR, RCC_CFGR_PPRE2, val);
}

/*
void stm32_clockbus_set_dividers(int a, int b) 
{
	__dividers[0] = a;
	__dividers[1] = b;
}*/

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
	             
	__hpre_divider = 1;
	__ppre1_divider = 1;	
	__ppre2_divider = 1;

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
			stm32_clockbus_freq[CLOCKBUS_APB1] = stm32_clockbus_freq[CLOCKBUS_HSI] / __ppre1_divider;
			stm32_clockbus_freq[CLOCKBUS_APB2] = stm32_clockbus_freq[CLOCKBUS_HSI] / __ppre2_divider;
			break;
		case 1:
			stm32_clockbus_freq[CLOCKBUS_APB1] = stm32_clockbus_freq[CLOCKBUS_HSE] / __ppre1_divider;
			stm32_clockbus_freq[CLOCKBUS_APB2] = stm32_clockbus_freq[CLOCKBUS_HSE] / __ppre2_divider;
			break;
		case 2:
			stm32_clockbus_freq[CLOCKBUS_APB1] = stm32_clockbus_freq[CLOCKBUS_PLL] / __ppre1_divider;
			stm32_clockbus_freq[CLOCKBUS_APB2] = stm32_clockbus_freq[CLOCKBUS_PLL] / __ppre2_divider;
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