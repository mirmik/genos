#include <systime/systime.h>
#include <igris/sync/syslock.h>

#include <igris/dprint.h>

volatile jiffies_t __jiffies = 0;
uint32_t system_frequency = 1;
uint32_t jiffies_to_millis = 1;

#define FREQSHIFT 8

jiffies_t jiffies() { 
	system_lock();
	jiffies_t ret = __jiffies;
	system_unlock();
	return ret; 
}

void systime_set_frequency(uint32_t freq) 
{
	system_frequency   = freq;	
	jiffies_to_millis  = ((uint32_t)1000 << FREQSHIFT) / freq;
}

jiffies_t millis() {
	return (jiffies() * jiffies_to_millis) >> FREQSHIFT;
}

void delay(double d) {
	jiffies_t n = millis();
	jiffies_t f = n + d;

	while(f - millis() > 0);
}

jiffies_t ms2jiffies(uint32_t ms) {
	return (ms << FREQSHIFT) / jiffies_to_millis;
}

void system_tick()
{ 
	++__jiffies; 
}