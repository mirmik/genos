/*	STM32 Start Label */

#include <asm/irq.h>
#include <string.h>

#include <asm/gpio.h>
#include <asm/rcc.h>
#include <drivers/gpio/gpio.h>

#include <periph/map.h>

typedef void (*func_ptr)(void); 

extern unsigned char _sdata;
extern unsigned char _edata;
extern unsigned char _sidata;

extern unsigned char _sbss;
extern unsigned char _ebss;

extern int main();

extern func_ptr __init_array_start[0], __init_array_end[0];
extern func_ptr __fini_array_start[0], __fini_array_end[0];

void cxx_invoke_init_array(void) {
	for ( func_ptr* func = __init_array_start; func != __init_array_end; func++ )	{
		(*func)();
	}
}

void cxx_invoke_fini_array(void) {
	for ( func_ptr* func = __fini_array_start; func != __fini_array_end; func++ )	{
		(*func)();
	}
}

void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
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

  /* Configure the Vector Table location add offset address ------------------*/
/*#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
//#else
//  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
//#endif
}

void __start(void) 
{
	irqs_disable();

	SystemInit();

	stm32l4_rcc_enable_gpio(GPIOB);

	stm32l4_gpio_mode(GPIOB, 0xFFFF, GPIO_MODE_OUTPUT);
	stm32l4_gpio_write(GPIOB, 0xFFFF, 1);

	irqs_disable();
	for( ; ; );
}