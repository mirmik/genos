#ifndef GENOS_KERNEL_SYSTIME_H
#define GENOS_KERNEL_SYSTIME_H

#include <inttypes.h>
#include <sys/cdefs.h>

// Обязательно должен быть знаковым.
// Аналог clock_t.
typedef int64_t jiffies_t;

extern volatile jiffies_t __jiffies;

__BEGIN_DECLS

void systime_set_frequency(uint32_t freq);
void system_tick();

jiffies_t jiffies();
jiffies_t millis();
jiffies_t micros();

void delay(double d);
	
jiffies_t ms2jiffies(uint32_t ms);

__END_DECLS

#endif