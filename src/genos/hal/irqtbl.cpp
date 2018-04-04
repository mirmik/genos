#include <genos/hal/do_irq.h>
#include <genos/hal/irqs.h>
#include <genos/hal/irqtbl.h>

#include <periph/irqdefs.h>

#include <gxx/debug/dprint.h>

unsigned char __interrupt_context = 0;
struct genos::hal::irqtbl::irq_record irqs_table[NR_IRQS];

void do_irq(uint8_t irq) {
	__interrupt_context++;
	irqs_table[irq].count++;
	irqs_table[irq].handler();
	__interrupt_context--;
}

unsigned char genos::hal::irqtbl::is_interrupt_context() {
	return __interrupt_context;
}

void genos::hal::irqtbl::irq_stub(void* irqno) {
	genos::hal::irqs::disable();
	dpr("IRQ_STUB_");
	dprptr(irqno);
	dln();
	while(1);
}

void genos::hal::irqtbl::init() {
	for (int i = 0; i < NR_IRQS; ++i) {
		irqs_table[i].handler = gxx::fastaction(irq_stub, (void*)i);
	}
}

void genos::hal::irqtbl::set_handler(int irqno, gxx::fastaction handler) {
	irqs_table[irqno].handler = handler;
}