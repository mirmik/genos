#ifndef GENOS_KERNEL_SYSTIME_H
#define GENOS_KERNEL_SYSTIME_H

#include <inttypes.h>
#include <asm/systime.h>

namespace systime { 
	void system_tick();

	uint32_t millis();
	uint32_t micros();	

	void delay(uint32_t d);
	void delayMicroseconds(uint16_t d);	
}

#endif