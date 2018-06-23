#ifndef GENOS_HAL_IRQTBL_H
#define GENOS_HAL_IRQTBL_H

#include <gxx/event/delegate.h>

namespace genos {
	namespace hal {
		namespace irqtbl {
			struct irq_record {
				gxx::fastaction handler;
				volatile uint16_t count;
			};
	
			unsigned char is_interrupt_context();
			void init();
			void set_handler(int irqno, gxx::fastaction handler);
	
			void irq_stub(void* irqno);
		}
	}
}

#endif