#ifndef GENOS_KERNEL_SYSTIME_H
#define GENOS_KERNEL_SYSTIME_H

#include <inttypes.h>

typedef uint32_t clock_t;

extern volatile clock_t __jiffies;

__BEGIN_DECLS

void systime_set_frequency(uint32_t freq);
void system_tick();

clock_t jiffies();
clock_t millis();
clock_t micros();

void delay(double d);
	
clock_t ms2jiffies(uint32_t ms);

__END_DECLS

#endif