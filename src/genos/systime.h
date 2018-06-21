#ifndef GENOS_KERNEL_SYSTIME_H
#define GENOS_KERNEL_SYSTIME_H

#include <inttypes.h>
//#include <asm/systime.h>

namespace systime { 
	using time_t = uint64_t;

	//extern volatile uint64_t __jiffies;
	extern uint32_t frequency;

	void system_tick();

	///jiffies
	time_t now();

/*	///Перевести число миллисекунд в jiffies.
	time_t milliseconds(uint32_t ms);

	///Перевести число микросекунд в jiffies.
	time_t microseconds(uint64_t ms);
*/

	time_t millis();
	time_t micros();

	//uint64_t jiffies();
	//uint32_t millis();
	//uint32_t micros();	

	void delay(double d);
	
	//void delayMicroseconds(uint16_t d);	


}

#endif