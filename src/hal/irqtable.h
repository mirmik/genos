#ifndef GENOS_HAL_IRQTBL_H
#define GENOS_HAL_IRQTBL_H

#include <hal/irq.h>

struct irq_record {
	irq_handler_t handler;
	void* handler_arg;
	volatile uint16_t count;
};
	
__BEGIN_DECLS

unsigned char is_interrupt_context();

void irqtable_init();

void irq_set_handler(int irqno, irq_handler_t handler, void* arg);

void irq_stub(void* irqno);

__END_DECLS

#ifdef __cplusplus
#include <igris/event/delegate.h>

namespace genos 
{
	namespace irqtable 
	{
		void set_handler(int irqno, irq_handler_t handler, void* arg);

		template<class T>
		void set_handler(int irqno, void(T::* handler)(), T* arg) 
		{
			auto dlg = igris::make_fastdelegate(handler, arg); 
			set_handler(irqno, dlg.extfunction, dlg.object);
		}
	}
}
#endif

#endif