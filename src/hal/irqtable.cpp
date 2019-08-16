#include <hal/do_irq.h>
#include <hal/irq.h>
#include <hal/irqtable.h>

#include <periph/irqdefs.h>

#include <igris/dprint.h>

volatile unsigned char __interrupt_context = 0;

// Длина таблицы определяется в periph/irqdefs.h
struct irq_record __irqtable[NR_IRQS];

void do_irq(uint8_t irq)
{
	__interrupt_context++;
	__irqtable[irq].count++;
	__irqtable[irq].handler(__irqtable[irq].handler_arg);
	__interrupt_context--;
}

unsigned char is_interrupt_context()
{
	return __interrupt_context;
}

void irqtable_stub(void* irqno)
{
	irqs_disable();
	dpr("IRQ_STUB_");
	dprln((uintptr_t)irqno);
	
	while (1);
}

void irqtable_init()
{
	for (int i = 0; i < NR_IRQS; ++i)
	{
		__irqtable[i].handler = &irqtable_stub;
		__irqtable[i].handler_arg = (void*)i;
		__irqtable[i].count = 0;
	}
}

void irqtable_set_handler(int irqno, irq_handler_t handler, void* handler_arg)
{
	__irqtable[irqno].handler = handler;
	__irqtable[irqno].handler_arg = handler_arg;
}

void irqtable_debug_print() 
{
	dprln("__irqtable:");
	for (int i = 0; i < NR_IRQS; ++i)
	{
		dpr(i); 
		dpr(": "); 
		dprptr((void*)__irqtable[i].handler);
		dpr(" ");
		dprptr(__irqtable[i].handler_arg);
		dpr(" ");
		dprln(__irqtable[i].count);		
	}	
}

uint16_t genos::irqtable::counter(int irqno) 
{
	auto save = irqs_save();	
	uint16_t count = __irqtable[irqno].count; 
	irqs_restore(save);

	return count;
}