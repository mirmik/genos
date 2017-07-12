#include <hal/arch.h>
#include <hal/irq.h>
#include <hal/systime.h>
#include <drivers/stm32/rcc.h>
#include <gxx/debug/dprint.h>

#include <kernel/diag.h>

void rcc_reset();

extern struct diag_ops diag_operations; 

//uint32_t generator_frequency;

//int stm32_diag_init();
int stm32_diag_putchar(char c);

void stm32_systime_init();


void arch_diag_init() {
//	diag_operations.init = stm32_diag_init;
	diag_operations.putc = stm32_diag_putchar;
	diag_operations.write = diag_write_stub;
	current_diag = &diag_operations;
	//diag_init();
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
unsigned arch_external_generator_init(unsigned Mkoeff, unsigned Nkoeff, unsigned Pkoeff, unsigned Qkoeff, unsigned HSE) {
	//generator_frequency = 168000;
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1
	   | RCC_CFGR_PPRE1_DIV1 
	   | RCC_CFGR_PPRE2_DIV1;
	
	//Запуск HSE и ожидание включения
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));

	//SYSCLK = HSE /M *N /P
	RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE | 
		  (RCC_PLLCFGR_PLLM & (Mkoeff << 0)) 
		| (RCC_PLLCFGR_PLLN & (Nkoeff << 6)) 		// 50 >= PLLN >= 432
		| (RCC_PLLCFGR_PLLP & (((Pkoeff >> 1) - 1) << 16))
		| (RCC_PLLCFGR_PLLQ & (Qkoeff << 24))
	);

	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));   

	//Тактируем шину от PLL
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (RCC_CFGR_SW_PLL);
	while((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

	return HSE / Mkoeff * Nkoeff / Pkoeff;
}

void arch_init() {
	volatile int i;

	rcc_reset();

	RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->APB1RSTR = 0xFFFFFFFF; 
	RCC->APB2RSTR = 0xFFFFFFFF;
	i = 1000; while(--i);
	
	RCC->AHB1RSTR = 0;
	RCC->APB1RSTR = 0; 
	RCC->APB2RSTR = 0;
	i = 1000; while(--i);

	arch_diag_init();
}

void arch_shutdown(arch_shutdown_mode_t mode) {
	switch(mode)
	{
		case ARCH_SHUTDOWN_MODE_HALT:
		break;
		case ARCH_SHUTDOWN_MODE_REBOOT:
		break;
		case ARCH_SHUTDOWN_MODE_ABORT:
		break;
	};
	global_irqs_disable();
	while(1);
}