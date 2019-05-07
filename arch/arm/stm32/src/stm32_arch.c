#include <hal/arch.h>
#include <hal/irq.h>

#include <asm/arch.h>
#include <asm/rcc.h>

#include <periph/map.h>
#include <periph/irqdefs.h>

#include <systime/systime.h>

#include <hal/irqtable.h>

#include <string.h>

void rcc_reset();

void arch_init()
{
	volatile int i;

	irqs_disable();
	rcc_reset();

	RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->APB1RSTR = 0xFFFFFFFF;
	RCC->APB2RSTR = 0xFFFFFFFF;
	i = 1000; while (--i);

	RCC->AHB1RSTR = 0;
	RCC->APB1RSTR = 0;
	RCC->APB2RSTR = 0;
	i = 1000; while (--i);

	//Активируем отладочную консоль на интерфейсе UART2
	//Не активно, потому что инициализацию терминала должен
	//выполнять board, ибо стандартного терминала нет.
//	rcc_enable_usart(USART2);
//	stm32_usart_setup(USART2, 115200, 'n', 8, 1);
//	stm32_diag_init(USART2);

	// Настроен на встроенный генератор 8МГц
	// Период - 1мс.
	stm32_systick_config(HSI_FREQ / 1000);
	systime_set_frequency(1000);

	// Инициализируем таблицу прерываний.
	// Возможно, прерывание системного таймера не должно идти 
	// через таблицу прерываний
	irqtable_init();
	irqtable_set_handler(STM32_IRQ_SYSTICK, system_tick, NULL);
}

void nvic_set_priority(int irqno, int prio);

int stm32_systick_config(uint32_t ticks)
{
	if ((ticks - 1) > SYSTICK_VALUE_MASK)  
		return 1; // reload value impossible
	
	SYSTICK->RVR = ticks - 1; // set reload register
	SYSTICK->CVR = 0; // reset current value
	
	// set max priority for systick interrupt
	//nvic_set_priority(STM32_IRQ_SYSTICK, (1 << NVIC_PRIO_BITS) - 1); 
	
	// enable systick irq and systick timer
	SYSTICK->CSR  = SYSTICK_CTRL_CLKSOURCE |
	                SYSTICK_CTRL_TICKINT |
	                SYSTICK_CTRL_ENABLE;
	
	// Success 
	return 0;
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
unsigned stm32_init_pll_clocking(struct stm32_pll_settings* s)
{
	// Включаем масштабирование регулятора напряжения.
	// Необходимо для высоких частот ???
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	/// Настраиваем задержки FLASH для работы при повышенной частоте.
	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	// Настраиваем делители в цепочках тактирования
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1
	             | RCC_CFGR_PPRE1_DIV2
	             | RCC_CFGR_PPRE2_DIV1;

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

void arch_shutdown(arch_shutdown_mode_t mode)
{
	switch (mode)
	{
		case ARCH_SHUTDOWN_MODE_HALT:
			break;
		case ARCH_SHUTDOWN_MODE_REBOOT:
			break;
		case ARCH_SHUTDOWN_MODE_ABORT:
			break;
	};
	irqs_disable();
	while (1);
}