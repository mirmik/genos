#include <genos/hal/irqs.h>
#include <avr/io.h>
#include <avr/interrupt.h>

namespace hal {
	namespace irqs {
		static inline void global_irqs_enable(void) {
			sei();
		}
		
		static inline irqstate_t global_irqs_save(void) {
			irqstate_t save = SREG;
			cli();
			return save;
		}
		
		static inline void global_irqs_restore(irqstate_t state) {
			SREG = state;
		}
		
		static inline void global_irqs_disable(void) {
			(void) global_irqs_save();
		}
	}
}