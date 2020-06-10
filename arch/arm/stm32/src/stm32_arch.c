#include <hal/arch.h>

#include <periph/map.h>
#include <periph/irqdefs.h>
#include <periph/coremap.h>

#include <asm/stm32_rcc.h>
#include <asm/stm32_systick.h>

#include <asm/irq.h>
#include <systime/systime.h>
#include <hal/irqtable.h>

#ifdef STM32F4XX
#define HSI_FREQ 16000000
#elif STM32G4XX
#define HSI_FREQ 16000000
#else
#error "undefined HSI frequency"
#endif

void stm32_sysinit();

static void init_systick_hsi() 
{
	stm32_systick_config(HSI_FREQ / 1000);
	sysclock_set_frequency(HSI_FREQ);
	init_delays(HSI_FREQ);
	systime_set_frequency(1000);

	irqtable_set_handler(SysTick_IRQn, system_tick, NULL);
}

void arch_init() 
{
	irqs_disable();

	stm32_clockbus_freq[CLOCKBUS_HSI] = HSI_FREQ;
	stm32_sysinit(); // Инициализация конкретной архитектуры

	irqtable_init();
	init_systick_hsi();
}