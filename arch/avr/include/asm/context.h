#ifndef GENOS_AVR_CONTEXT_H
#define GENOS_AVR_CONTEXT_H

#include <genos/hal/subst.h>
#include "inttypes.h"

struct context {
	uint8_t regs[32];
	uint16_t pc;
	uint16_t sp;
	uint8_t control;
};

#endif