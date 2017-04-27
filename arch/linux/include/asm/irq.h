#ifndef ARCH_LINUX_IRQ_H_STUB
#define ARCH_LINUX_IRQ_H_STUB

#include <compiler.h>

#define IRQS_TOTAL 0
typedef unsigned char irqstate_t;

__BEGIN_DECLS

void global_irq_enable(void);
irqstate_t global_irq_save(void);
void global_irq_restore(irqstate_t state);
void global_irq_disable(void);

__END_DECLS

#endif