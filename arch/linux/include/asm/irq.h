#ifndef ARCH_LINUX_IRQ_H_STUB
#define ARCH_LINUX_IRQ_H_STUB

#define IRQS_TOTAL 0
typedef int irqstate_t;

#define global_irq_restore(save) (void)0
#define global_irq_save() (irqstate_t)0;

#define global_irq_disable() (irqstate_t)0;
//#define global_irq_save() (irqstate_t)0;

#endif