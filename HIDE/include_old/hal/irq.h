#ifndef HAL_IPL_H_
#define HAL_IPL_H_

#include <genos/compiler.h>
#include <asm/irq.h>

__BEGIN_DECLS

void irq_enable(int irqno);
void irq_disable(int irqno);

/*define as static function in asm/irq.h*/

//void global_irqs_enable(void);
//irqstate_t global_irqs_save(void);
//void global_irqs_restore(irqstate_t state);
//void global_irqs_disable(void);

__END_DECLS

#endif /* HAL_IPL_H_ */
