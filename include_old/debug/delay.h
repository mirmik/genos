#ifndef DEBUG_DELAY_H
#define DEBUG_DELAY_H

#include <stdint.h>
#include "genos_configure.h"

static inline void debug_delay(float arg) {
	volatile uint64_t count = arg * DEBUG_DELAY_CALIBRATE;
	while (--count);
}

#endif