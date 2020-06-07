#include <periph/map.h>
#include <periph/coremap.h>

#include <asm/stm32_clockbus.h>
#include <asm/stm32_rcc.h>

//#define HSI_FREQ 8000000

void stm32_sysinit(void)
{
	volatile int i;

	/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

	RCC->CFGR = 0x00000000;
	RCC->PLLCFGR = 0x00001000;
	RCC->CR &= (uint32_t)0xFFFBFFFF;
	RCC->CIER = 0x00000000;

	stm32_rcc_reset();
	i = 1000; while (--i);

	RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->AHB2RSTR = 0xFFFFFFFF;
	RCC->AHB3RSTR = 0xFFFFFFFF;
	RCC->APB1RSTR1 = 0xFFFFFFFF;
	RCC->APB1RSTR2 = 0xFFFFFFFF;
	RCC->APB2RSTR = 0xFFFFFFFF;
	i = 1000; while (--i);

	RCC->AHB1RSTR = 0x00000000;
	RCC->AHB2RSTR = 0x00000000;
	RCC->AHB3RSTR = 0x00000000;
	RCC->APB1RSTR1 = 0x00000000;
	RCC->APB1RSTR2 = 0x00000000;
	RCC->APB2RSTR = 0x00000000;
	i = 1000; while (--i);

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1
	             | RCC_CFGR_PPRE1_DIV1
	             | RCC_CFGR_PPRE2_DIV1;
}
