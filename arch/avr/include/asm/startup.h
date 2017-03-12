#ifndef GENOS_STARTUP_H
#define GENOS_STARTUP_H

#include "genos_configure.h"

#if defined(CHIP_ATMEGA2560)

#define STACKBASE (0x21FF)
#define RESET_STACK() SP = STACKBASE

#define HAVE_RAMPZ 1
#else 
#error "unrecognized chip"
#endif

#endif