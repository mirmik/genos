#include <avr/io.h>
#include <avr/interrupt.h>
#include <igris/time/jiffies-systime.h>

ISR (TIMER0_OVF_vect)
{
	jiffies_increment();
}