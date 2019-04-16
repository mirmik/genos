#ifndef GENOS_KERNEL_SYSTIME_H
#define GENOS_KERNEL_SYSTIME_H

#include <inttypes.h>

typedef int64_t time_t;

extern volatile uint64_t __jiffies;

__BEGIN_DECLS

void systime_set_frequency(uint32_t freq);

static inline void system_tick() { ++__jiffies; }

time_t jiffies();
time_t millis();
time_t micros();

void delay(double d);
	
time_t ms2jiffies(uint32_t ms);

__END_DECLS

#endif