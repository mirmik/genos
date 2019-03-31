#include <genos/time/systime.h>
#include <gxx/debug/dprint.h>
#include <igris/sync/syslock.h>

volatile uint64_t __jiffies;
uint32_t systime_frequency;

__BEGIN_DECLS


///jiffies
time_t jiffies() { 
	system_lock();
	time_t ret = __jiffies;
	system_unlock();
	return ret; 
}
/*
///Перевести число миллисекунд в jiffies.
systime::time_t systime::milliseconds(uint32_t ms) { return ms * frequency / 1000; }

///Перевести число микросекунд в jiffies.
systime::time_t systime::microseconds(uint64_t ms) { return ms * frequency / 1000000; }
*/

time_t millis() {
	return jiffies() * 1000 / systime_frequency;
}

void delay(double d) {
	auto n = __jiffies;
	auto f = n + d * systime_frequency;
	while(__jiffies < f);
}

time_t ms2jiffies(uint32_t ms) {
	return ms * systime_frequency / 1000;
}

__END_DECLS