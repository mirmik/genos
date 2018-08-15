#ifndef GENOS_HAL_IRQTBL_H
#define GENOS_HAL_IRQTBL_H

#include <hal/irq.h>

struct irq_record {
	irq_handler_t handler;
	void* handler_arg;
	volatile uint16_t count;
};
	
__BEGIN_DECLS

unsigned char is_interrupt_context();

void irqtable_init();

void irq_set_handler(int irqno, irq_handler_t handler, void* arg);

void irq_stub(void* irqno);

__END_DECLS

#endif