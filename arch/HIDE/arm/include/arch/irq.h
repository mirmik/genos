#ifndef STM32_IRQ_H
#define STM32_IRQ_H

#include <inttypes.h>

typedef unsigned int irqstate_t;

static inline void irqs_enable(void) {
	__asm__ __volatile__ (
		"cpsie i \n\t");

}

static inline irqstate_t irqs_save(void) {
	register uint32_t r;
	__asm__ __volatile__ (
		"mrs %0, PRIMASK;\n\t"
		"cpsid i \n\t"
		: "=r"(r));
	return r;
}

static inline void irqs_restore(irqstate_t state) {
	__asm__ __volatile__ (
		"msr PRIMASK, %0;\n\t"
		:
		: "r"(state));
}

static inline void irqs_disable(void) {
	(void) irqs_save();
}

#endif
