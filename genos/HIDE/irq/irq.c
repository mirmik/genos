
#include <gxx/util/stub.h>

struct irq_record irqs_table[NR_IRQS];

void irqtable_init() {
	for (int i = 0; i < NR_IRQS; ++i) {
		irqs_table[i].handler = irq_stub;
		irqs_table[i].argument = (void*)i;
	}
}

void set_irq_handler(int irqno, irq_handler_t handler, void* arg) {
	irqs_table[irqno].handler = handler;
	irqs_table[irqno].argument = arg;
}