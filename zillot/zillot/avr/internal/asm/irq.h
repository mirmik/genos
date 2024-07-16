#ifndef ARCH_IRQS_H
#define ARCH_IRQS_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <igris/compiler.h>

typedef unsigned char irqstate_t;

__BEGIN_DECLS

static inline void irqs_enable(void) {
	sei();
}

static inline irqstate_t irqs_save(void) {
	irqstate_t save = SREG;
	cli();
	return save;
}

static inline void irqs_restore(irqstate_t state) {
	SREG = state;
}

static inline void irqs_disable(void) {
	(void) irqs_save();
}

__END_DECLS


#endif