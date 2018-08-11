#include <genos/time/systime.h>
#include <gxx/debug/dprint.h>
#include <gxx/syslock.h>

static volatile uint64_t __jiffies;
uint32_t systime::frequency;

//void systime::system_tick() __attribute__((weak));
void systime::system_tick() { ++__jiffies; }

///jiffies
systime::time_t systime::jiffies() { 
	gxx::system_lock();
	systime::time_t ret = __jiffies;
	gxx::system_unlock();
	return ret; 
}
/*
///Перевести число миллисекунд в jiffies.
systime::time_t systime::milliseconds(uint32_t ms) { return ms * frequency / 1000; }

///Перевести число микросекунд в jiffies.
systime::time_t systime::microseconds(uint64_t ms) { return ms * frequency / 1000000; }
*/

systime::time_t systime::millis() {
	return systime::jiffies() * 1000 / systime::frequency;
}

void systime::delay(double d) {
	auto n = now();
	auto f = n + d * frequency;
	while(now() < f);
}

systime::time_t systime::ms2j(uint32_t ms) {
	return ms * systime::frequency / 1000;
}