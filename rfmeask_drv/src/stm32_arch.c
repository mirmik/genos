#include <asm/irq.h>
#include <igris/dprint.h>
#include <igris/time/jiffies-systime.h>
#include <stm32_arch.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/stm32_clockbus.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_systick.h>

#include <stm32xxxx.h>

void arch_init()
{
    volatile int i;

    irqs_disable();
    //	rcc_reset();

    RCC->AHB1RSTR = 0xFFFFFFFF;
    RCC->APB1RSTR = 0xFFFFFFFF;
    RCC->APB2RSTR = 0xFFFFFFFF;
    i = 1000;
    while (--i)
        ;

    RCC->AHB1RSTR = 0;
    RCC->APB1RSTR = 0;
    RCC->APB2RSTR = 0;
    i = 1000;
    while (--i)
        ;

    // Настроен на встроенный генератор 8МГц
    // Период - 1мс.
    stm32_systick_config(HSI_FREQ / 1000);
    systime_set_frequency(1000);

    // Инициализируем таблицу прерываний.
    // Возможно, прерывание системного таймера не должно идти
    // через таблицу прерываний
    irqtable_init();
    irqtable_set_handler(SysTick_IRQn, jiffies_increment, NULL);
}