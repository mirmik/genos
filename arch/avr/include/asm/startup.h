#ifndef GENOS_AVR_STARTUP_H
#define GENOS_AVR_STARTUP_H

#include <avr/io.h>

#define STACKBASE (RAMEND)
#define RESET_STACK() SP = STACKBASE
#define SET_STACK_POINTER(arg) SP = arg

#if defined(CHIP_ATMEGA2560)
#	define HAVE_RAMPZ
#	define HAVE_EIND
#elif defined(CHIP_ATMEGA328P)
#else 
#	error "unrecognized chip"
#endif

#endif