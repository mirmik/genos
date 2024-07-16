#ifndef ZILLOT_ARMV7E_NVIC_M
#define ZILLOT_ARMV7E_NVIC_M

#include <stm32xxxx.h> /// < Почему это в armv7e-m?
#include <igris/compiler.h>

__BEGIN_DECLS

void nvic_enable_irq(uint32_t irqno);
void nvic_set_priority(int irqno, int32_t prio);

__END_DECLS

#endif