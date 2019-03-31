#include <systime/systime.h>
#include <igris/sync/syslock.h>

volatile uint64_t __jiffies;
uint32_t systime_frequency;

void system_tick() { ++__jiffies; }

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
	time_t n = millis();
	time_t f = n + d;
	while(f - millis() > 0);
}

time_t ms2jiffies(uint32_t ms) {
	return ms * systime_frequency / 1000;
}