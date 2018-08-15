#ifndef STM32_IPL_H
#define STM32_IPL_H

#include <hal/ipl.h>
#include <inttypes.h>

typedef unsigned int ipl_t;

static inline void ipl_init(void) {
	__asm__ __volatile__ (
		"cpsie i \n\t");

}

static inline ipl_t ipl_save(void) {
	register uint32_t r;
	__asm__ __volatile__ (
		"mrs %0, PRIMASK;\n\t"
		"cpsid i \n\t"
		: "=r"(r));
	return r;
}

static inline void ipl_restore(ipl_t ipl) {
	__asm__ __volatile__ (
		"msr PRIMASK, %0;\n\t"
		:
		: "r"(ipl));
}

#endif
