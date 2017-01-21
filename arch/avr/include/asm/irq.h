#ifndef STM32_IRQ_H
#define STM32_IRQ_H

#include "avr/io.h"
#include "avr/interrupt.h"

typedef unsigned char irqstate_t;

static inline void global_irq_enable(void) {
	sei();
}

static inline irqstate_t global_irq_save(void) {
	irqstate_t save = SREG;
	cli();
	return save;
}

static inline void global_irq_restore(irqstate_t state) {
	SREG = state;
}

static inline void global_irq_disable(void) {
	(void) global_irq_save();
}

#endif
