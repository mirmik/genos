#ifndef GENOS_IRQ_H
#define GENOS_IRQ_H

#include <compiler.h>
#include <kernel/panic.h>
#include <inttypes.h>

#include <periph/irqdefs.h>

typedef void(*IRQHandler)(void*);

struct IRQTableRecord {
	IRQHandler 	handler;
	void* 		argument;
	volatile uint16_t count;
};

__BEGIN_DECLS

extern struct IRQTableRecord IRQTable[NR_IRQS];

void do_irq(uint8_t irq) __attribute__((section(".handlers")));
unsigned char is_interrupt_context();

void irqtable_init();
void setIRQHandler(int irqno, IRQHandler handler, void* arg);

void irq_stub(void* irqno);

void debug_print_irq_table_info();

__END_DECLS

#endif