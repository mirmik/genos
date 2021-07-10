#include <periph/map.h>
#include <periph/coremap.h>

#include <asm/stm32_rcc.h>

void stm32_sysinit(void)
{
	volatile int i;

	/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif
	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set MSION bit */
	RCC->CR |= RCC_CR_MSION;

	/* Reset CFGR register */
	RCC->CFGR = 0x00000000;

	/* Reset HSEON, CSSON , HSION, and PLLON bits */
	RCC->CR &= (uint32_t)0xEAF6FFFF;

	/* Reset PLLCFGR register */
	RCC->PLLCFGR = 0x00001000;

	/* Reset HSEBYP bit */
	RCC->CR &= (uint32_t)0xFFFBFFFF;

	/* Disable all interrupts */
	RCC->CIER = 0x00000000;

	stm32_rcc_reset();

 	RCC->AHB1RSTR = 0xFFFFFFFF;
  	RCC->AHB2RSTR = 0xFFFFFFFF;    
  	RCC->AHB3RSTR = 0xFFFFFFFF;    
	RCC->APB1RSTR1 = 0xFFFFFFFF;   
  	RCC->APB1RSTR2 = 0xFFFFFFFF;   
 	RCC->APB2RSTR= 0xFFFFFFFF;
	i = 1000; while (--i);

 	RCC->AHB1RSTR = 0;
  	RCC->AHB2RSTR = 0;    
  	RCC->AHB3RSTR = 0;    
	RCC->APB1RSTR1 = 0;   
  	RCC->APB1RSTR2 = 0;   
 	RCC->APB2RSTR= 0;
	i = 1000; while (--i);
}
