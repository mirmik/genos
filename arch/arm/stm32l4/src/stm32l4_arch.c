#include <asm/arch.h>
#include <asm/rcc.h>

#include <hal/irqtable.h>
#include <periph/map.h>
#include <cmsis_core/core_cm4.h>
#include <systime/systime.h>
#include <igris/util/bits.h>

const uint32_t LSI_DECLARED_FREQ = 32000;  
const uint32_t HSI_DECLARED_FREQ = 16000000;
uint32_t SYSCLK_DECLARED_FREQ = -1;
uint32_t APB1_DECLARED_FREQ = -1;
uint32_t APB2_DECLARED_FREQ = -1;

int32_t stm32_declared_clockbus_freq[3] = { 1, 1, 1 };

void SystemInit(void)
{
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
}

int stm32_systick_config(uint32_t ticks)
{
	//if ((ticks - 1) > SYSTICK_VALUE_MASK)
	//	return 1; // reload value impossible

	SysTick->LOAD = ticks - 1; // set reload register
	SysTick->VAL = 0; // reset current value

	// set max priority for systick interrupt
	//nvic_set_priority(STM32_IRQ_SYSTICK, (1 << NVIC_PRIO_BITS) - 1);

	// enable systick irq and systick timer
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
	                SysTick_CTRL_TICKINT_Msk |
	                SysTick_CTRL_ENABLE_Msk;

	// Success
	return 0;
}

void stm32l4_clock_source_hse() 
{
	// Запуск HSE и ожидание включения
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Тактируем шину от PLL, ждём подтверждения.
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (RCC_CFGR_SW_HSE);
	while ((RCC->CFGR & RCC_CFGR_SWS_HSE) != RCC_CFGR_SWS_PLL);
}

void arch_init()
{
	volatile int i;

	/*RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->AHB2RSTR = 0xFFFFFFFF;
	RCC->APB1RSTR1 = 0xFFFFFFFF;
	RCC->APB1RSTR2 = 0xFFFFFFFF;
	RCC->APB2RSTR = 0xFFFFFFFF;
	i = 1000; while (--i);

	RCC->AHB1RSTR = 0;
	RCC->AHB2RSTR = 0;
	RCC->APB1RSTR1 = 0;
	RCC->APB1RSTR2 = 0;
	RCC->APB2RSTR = 0;
	i = 1000; while (--i);*/

	SystemInit();

	// Соединяем SYSCLK с HSI
	RCC->CR |= RCC_CR_HSION;
	bits_assign(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSI);
	SYSCLK_DECLARED_FREQ = HSI_DECLARED_FREQ;

	// Устанавливаем предделители.
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1
	             | RCC_CFGR_PPRE1_DIV1
	             | RCC_CFGR_PPRE2_DIV1;
	APB1_DECLARED_FREQ = HSI_DECLARED_FREQ;
	APB2_DECLARED_FREQ = HSI_DECLARED_FREQ;

	// Настроен на встроенный генератор 16МГц
	// Период - 1мс.
	stm32_systick_config(SYSCLK_DECLARED_FREQ / 1000);
	sysclock_set_frequency(SYSCLK_DECLARED_FREQ);
	init_delays(SYSCLK_DECLARED_FREQ);
	systime_set_frequency(1000);

	// Инициализируем таблицу прерываний.
	// Возможно, прерывание системного таймера не должно идти
	// через таблицу прерываний
	irqtable_init();
	irqtable_set_handler(SysTick_IRQn, system_tick, NULL);
}






/**	Инициализация внешнего генератора.
 *	Частота расщитывается по формуле: SYSCLK = HSE / Mkoeff * Nkoeff / Pkoeff
 *	HSE - справочный параметр на основе которой функция расчитывает установленную частоту.
 *	Диапазоны допустимых значений:
 *	M : TODO
 *	N : 50 ... 432
 *	P : TODO
 *	---
 *	Q определяет выход 48Mhz, предназначенный для контроля частоты.  48MHzOutput = HSE / Mkoeff * Nkoeff / Pkoeff
 */
//unsigned stm32_clock_(unsigned Mkoeff, unsigned Nkoeff, unsigned Pkoeff, unsigned Qkoeff, unsigned HSE)
unsigned stm32l4_init_pll_clocking(struct stm32l4_pll_settings* s)
{
	// Включаем масштабирование регулятора напряжения.
	// Необходимо для высоких частот ???
	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
	//PWR->CR1 |= PWR_CR1_VOS;

	/// Настраиваем задержки FLASH для работы при повышенной частоте.
	//FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_4WS;

	// Настраиваем делители в цепочках тактирования
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1
	             | RCC_CFGR_PPRE1_DIV2
	             | RCC_CFGR_PPRE2_DIV1;

	// Запуск HSE и ожидание включения
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Конфигурируем PLL, выбирая в качестве источника внешний кварц.
	RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE |
	                (RCC_PLLCFGR_PLLM & (2 << RCC_PLLCFGR_PLLM_Pos))
	                | (RCC_PLLCFGR_PLLN & (16 << RCC_PLLCFGR_PLLN_Pos))
	                | (RCC_PLLCFGR_PLLP & (RCC_PLLCFGR_PLLPDIV_4 << RCC_PLLCFGR_PLLP_Pos))
	                | (RCC_PLLCFGR_PLLR & (0b10 << RCC_PLLCFGR_PLLR_Pos))
	               	| (RCC_PLLCFGR_PLLQ & (0b10 << RCC_PLLCFGR_PLLQ_Pos))
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
