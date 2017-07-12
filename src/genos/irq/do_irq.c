#include <genos/irq.h>
#include <gxx/debug/dprint.h>

unsigned char __interrupt_context = 0;

void do_irq(uint8_t irq) {
	__interrupt_context++;
	irqs_table[irq].count++;
	irqs_table[irq].handler(irqs_table[irq].argument);
	__interrupt_context--;
}

unsigned char is_interrupt_context() {
	return __interrupt_context;
}