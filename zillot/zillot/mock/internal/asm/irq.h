#ifndef STUBARCH_ASM_IRQ_H
#define STUBARCH_ASM_IRQ_H

typedef int irqstate_t;

#define irqs_enable() (0)
#define irqs_disable() (0)
#define irqs_save() (0)
#define irqs_restore(arg) ((void)arg)

#endif