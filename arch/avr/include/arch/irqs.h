#ifndef ARCH_IRQS_H
#define ARCH_IRQS_H

#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef __cplusplus

namespace arch {
	namespace irqs {
		using save_t = uint8_t;

		static inline void enable(void) {
			sei();
		}
		
		static inline uint8_t save(void) {
			uint8_t save = SREG;
			cli();
			return save;
		}
		
		static inline void restore(uint8_t state) {
			SREG = state;
		}
		
		static inline void disable(void) {
			(void) save();
		}
	}
}

#endif

//C VERSION
typedef unsigned char irqstate_t;

__BEGIN_DECLS
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
__END_DECLS


#endif