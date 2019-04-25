#include <systime/systime.h>
#include <igris/sync/syslock.h>

#include <igris/dprint.h>

volatile uint64_t __jiffies;
uint32_t system_frequency = 1;
uint32_t jiffies_to_millis = 1;

#define FREQSHIFT 8

time_t jiffies() { 
	system_lock();
	time_t ret = __jiffies;
	system_unlock();
	return ret; 
}

void systime_set_frequency(uint32_t freq) 
{
	system_frequency   = freq;	
	jiffies_to_millis  = (1000 << FREQSHIFT) / freq;
}

time_t millis() {
	return (jiffies() * jiffies_to_millis) >> FREQSHIFT;
}

void delay(double d) {
	time_t n = millis();
	time_t f = n + d;
	while(f - millis() > 0);
}

time_t ms2jiffies(uint32_t ms) {
	return (ms << FREQSHIFT) / jiffies_to_millis;
}