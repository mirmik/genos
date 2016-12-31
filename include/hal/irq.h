#ifndef HAL_IPL_H_
#define HAL_IPL_H_

#include <compiler.h>
#include <asm/irq.h>

__BEGIN_DECLS

void irq_enable(int irqno);
void irq_disable(int irqno);

/*define as static function in asm/irq.h*/

//void global_irq_enable(void);
//irqstate_t global_irq_save(void);
//void global_irq_restore(irqstate_t state);
//void global_irq_disable(void);

__END_DECLS

#endif /* HAL_IPL_H_ */
