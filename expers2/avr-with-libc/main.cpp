#include <genos/ktimer.h>
#include <genos/sched.h>

#include <avr/io.h>
#include <hal/irqtable.h>

int main() 
{
//	irqtable_init();
	schedee_manager_init();

	DDRB = 0xFF;
	PORTB= 0xFF;
	while(1);
}