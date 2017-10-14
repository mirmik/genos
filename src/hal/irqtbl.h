#ifndef GENOS_HAL_IRQTBL_H
#define GENOS_HAL_IRQTBL_H

#include <gxx/event/delegate.h>

namespace hal {
	namespace irqtbl {
		struct irq_record {
			gxx::action handler;
			volatile uint16_t count;
		};

		unsigned char is_interrupt_context();
		void init();
		void set_handler(int irqno, gxx::action handler);

		void irq_stub(void* irqno);
	}
}

#endif