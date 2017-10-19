#ifndef GENOS_IRQ_H
#define GENOS_IRQ_H

#include <sys/cdefs.h>
#include <genos/panic.h>
#include <inttypes.h>

#include <periph/irqdefs.h>

typedef void(*IRQHandler)(void*);
typedef void(*irq_handler_t)(void*);

struct irq_record {
	irq_handler_t 	handler;
	void* 		argument;
	volatile uint16_t count;
};

__BEGIN_DECLS

extern struct irq_record irqs_table[NR_IRQS];

void do_irq(uint8_t irq) __attribute__((section(".handlers")));
unsigned char is_interrupt_context();

void irqtable_init();
void setIRQHandler(int irqno, irq_handler_t handler, void* arg);
#define set_irq_handler(a,b,c) setIRQHandler(a,b,c)

void irq_stub(void* irqno);

void debug_print_irq_table_info();

__END_DECLS

#endif