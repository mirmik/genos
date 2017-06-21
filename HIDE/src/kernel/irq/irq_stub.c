#include <kernel/irq.h>
#include <kernel/panic.h>

#include <stdint.h>

void irq_stub(void* irqno) {
	global_irqs_disable();
	debug_print("IRQ_STUB_");
	debug_printhex_ptr(irqno);
	dln();
	while(1);
}