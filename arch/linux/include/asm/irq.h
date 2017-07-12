#ifndef ARCH_LINUX_IRQ_H_STUB
#define ARCH_LINUX_IRQ_H_STUB

#include <genos/compiler.h>

#define IRQS_TOTAL 0
typedef unsigned char irqstate_t;

__BEGIN_DECLS

void global_irqs_enable(void);
irqstate_t global_irqs_save(void);
void global_irqs_restore(irqstate_t state);
void global_irqs_disable(void);

__END_DECLS

#endif