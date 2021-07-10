#ifndef GENOS_CPU_DELAY_H
#define GENOS_CPU_DELAY_H

#include <stdint.h>
#include <igris/compiler.h>

__BEGIN_DECLS

static inline void cpu_delay(uint64_t ticks) 
{
	volatile uint64_t t = ticks;
	while (--t);
}

__END_DECLS

#endif