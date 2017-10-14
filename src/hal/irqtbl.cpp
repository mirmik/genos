#include <hal/do_irq.h>
#include <hal/irqs.h>
#include <hal/irqtbl.h>
#include <periph/irqdefs.h>

#include <gxx/debug/dprint.h>

unsigned char __interrupt_context = 0;
struct hal::irqtbl::irq_record irqs_table[NR_IRQS];

void do_irq(uint8_t irq) {
	__interrupt_context++;
	irqs_table[irq].count++;
	irqs_table[irq].handler();
	__interrupt_context--;
}

unsigned char hal::irqtbl::is_interrupt_context() {
	return __interrupt_context;
}

void hal::irqtbl::irq_stub(void* irqno) {
	hal::irqs::disable();
	dpr("IRQ_STUB_");
	dprptr(irqno);
	dln();
	while(1);
}

void hal::irqtbl::init() {
	for (int i = 0; i < NR_IRQS; ++i) {
		irqs_table[i].handler = gxx::action(irq_stub, (void*)i);
	}
}

void hal::irqtbl::set_handler(int irqno, gxx::action handler) {
	irqs_table[irqno].handler = handler;
}