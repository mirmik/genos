#ifndef STM32_IRQ_H
#define STM32_IRQ_H

#include <inttypes.h>

typedef unsigned int irqstate_t;

static inline void global_irqs_enable(void) {
	__asm__ __volatile__ (
		"cpsie i \n\t");

}

static inline irqstate_t global_irqs_save(void) {
	register uint32_t r;
	__asm__ __volatile__ (
		"mrs %0, PRIMASK;\n\t"
		"cpsid i \n\t"
		: "=r"(r));
	return r;
}

static inline void global_irqs_restore(irqstate_t state) {
	__asm__ __volatile__ (
		"msr PRIMASK, %0;\n\t"
		:
		: "r"(state));
}

static inline void global_irqs_disable(void) {
	(void) global_irqs_save();
}

#endif
