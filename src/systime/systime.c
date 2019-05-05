#include <systime/systime.h>
#include <igris/sync/syslock.h>

#include <igris/dprint.h>

volatile clock_t __jiffies = 0;
uint32_t system_frequency = 1;
uint32_t jiffies_to_millis = 1;

#define FREQSHIFT 8

clock_t jiffies() { 
	system_lock();
	clock_t ret = __jiffies;
	system_unlock();
	return ret; 
}

void systime_set_frequency(uint32_t freq) 
{
	system_frequency   = freq;	
	jiffies_to_millis  = ((uint32_t)1000 << FREQSHIFT) / freq;
}

clock_t millis() {
	return (jiffies() * jiffies_to_millis) >> FREQSHIFT;
}

void delay(double d) {
	clock_t n = millis();
	clock_t f = n + d;
	while(f - millis() > 0);
}

clock_t ms2jiffies(uint32_t ms) {
	return (ms << FREQSHIFT) / jiffies_to_millis;
}

void system_tick()
{ 
	++__jiffies; 
}