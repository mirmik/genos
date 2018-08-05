#ifndef GENOS_AVR_MAP_H
#define GENOS_AVR_MAP_H

#if defined(CHIP_ATMEGA2560)
#	include <periph/impls/atmega2560_map.h>
#elif defined(CHIP_ATMEGA328P)
#	include <periph/impls/atmega328p_map.h>
#endif

#endif