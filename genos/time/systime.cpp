#include <genos/time/systime.h>
#include <gxx/debug/dprint.h>

static volatile uint64_t __jiffies;
uint32_t systime::frequency;

//void systime::system_tick() __attribute__((weak));
void systime::system_tick() { ++__jiffies; }

///jiffies
systime::time_t systime::now() { return __jiffies; }
/*
///Перевести число миллисекунд в jiffies.
systime::time_t systime::milliseconds(uint32_t ms) { return ms * frequency / 1000; }

///Перевести число микросекунд в jiffies.
systime::time_t systime::microseconds(uint64_t ms) { return ms * frequency / 1000000; }
*/

systime::time_t systime::millis() {
	return __jiffies * 1000 / systime::frequency;
}

void systime::delay(double d) {
	auto n = now();
	auto f = n + d * frequency;
	while(now() < f);
}