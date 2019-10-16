#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>

#include <asm/gpio.h>
#include <asm/rcc.h>

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

}
int main()
{

	stm32l4_rcc_enable_gpio(GPIOB);

	stm32l4_gpio_mode(GPIOB, 0xFFFF, GPIO_MODE_OUTPUT);
	stm32l4_gpio_write(GPIOB, 0xFFFF, 1);

	irqs_disable();
	for( ; ; );

	/*board_init();

	irqs_enable();

	while(1) 
	{
		board::sysled.toggle();
		delay(500);
	}*/
}