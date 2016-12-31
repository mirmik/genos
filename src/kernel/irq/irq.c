#include <kernel/irq.h>
#include <util/stub.h>

struct IRQTableRecord IRQTable[NR_IRQS];

void irqtable_init() {
	for (int i = 0; i < NR_IRQS; ++i) {
		IRQTable[i].handler = irq_stub;
		IRQTable[i].argument = (void*)i;
	}
}

void setIRQHandler(int irqno, IRQHandler handler, void* arg) {
	IRQTable[irqno].handler = handler;
	IRQTable[irqno].argument = arg;
}