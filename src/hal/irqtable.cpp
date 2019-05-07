#include <hal/do_irq.h>
#include <hal/irq.h>
#include <hal/irqtable.h>

#include <periph/irqdefs.h>

#include <igris/dprint.h>

volatile unsigned char __interrupt_context = 0;

// Длина таблицы определяется в periph/irqdefs.h
struct irq_record irqtable[NR_IRQS];

void do_irq(uint8_t irq)
{
	__interrupt_context++;
	irqtable[irq].count++;
	irqtable[irq].handler(irqtable[irq].handler_arg);
	__interrupt_context--;
}

unsigned char is_interrupt_context()
{
	return __interrupt_context;
}

void irqtable_stub(void* irqno)
{
	irqs_disable();
	debug_print("IRQ_STUB_");
	debug_printhex_ptr(irqno);
	debug_print_line("");

	while (1);
}

void irqtable_init()
{
	for (int i = 0; i < NR_IRQS; ++i)
	{
		irqtable[i].handler = &irqtable_stub;
		irqtable[i].handler_arg = (void*)i;
		irqtable[i].count = 0;
	}
}

void irqtable_set_handler(int irqno, irq_handler_t handler, void* handler_arg)
{
	irqtable[irqno].handler = handler;
	irqtable[irqno].handler_arg = handler_arg;
}

void irqtable_debug_print() 
{
	dprln("irqtable:");
	for (int i = 0; i < NR_IRQS; ++i)
	{
		dpr(i); 
		dpr(": "); 
		dprptr((void*)irqtable[i].handler);
		dpr(" ");
		dprptr(irqtable[i].handler_arg);
		dpr(" ");
		dprln(irqtable[i].count);		
	}	
}