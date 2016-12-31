#ifndef GENOS_AVR_IPL_H
#define GENOS_AVR_IPL_H

#include "hal/ipl.h"
#include "avr/io.h"
#include "avr/interrupt.h"

typedef unsigned char ipl_t;

static inline void ipl_init(void)
{
	sei();
};

static inline ipl_t ipl_save(void)
{
	ipl_t save = SREG;
	cli();
	return save;
};

static inline void ipl_restore(ipl_t ipl)
{
	SREG = ipl;
};


#endif