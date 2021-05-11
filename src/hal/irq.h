#ifndef HAL_IPL_H_
#define HAL_IPL_H_

#include <igris/compiler.h>

typedef void(*irq_handler_t)(void*);

__BEGIN_DECLS

void irq_enable(int irqno);
void irq_disable(int irqno);

//Defined in asm/irq.h
//void irqs_enable(void);
//irqstate_t irqs_save(void);
//void irqs_restore(irqstate_t state);
//void irqs_disable(void);

unsigned char is_interrupt_context();

__END_DECLS

#if !__NOARCH
#include <asm/irq.h>
#else
__BEGIN_DECLS
void irqs_enable() {}
void irqs_disable() {}
__END_DECLS
#endif

#endif /* HAL_IPL_H_ */
