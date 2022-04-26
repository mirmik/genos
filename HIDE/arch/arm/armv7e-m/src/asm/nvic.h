#ifndef ARMV7E_M_ASM_NVIC_H
#define ARMV7E_M_ASM_NVIC_H

#include <periph/map.h>
#include <assert.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

void dprint_dump_nvic();
void nvic_enable_irq(uint32_t irqno);
void nvic_set_priority(int irqno, int32_t prio);

__END_DECLS

#endif