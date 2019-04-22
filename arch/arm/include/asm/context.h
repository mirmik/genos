#ifndef ARM_CONTEXT_H
#define ARM_CONTEXT_H

#include "inttypes.h"

struct context {
	uint32_t r[13];
	uint32_t lr;
	uint32_t sp;
	uint32_t control;
};

#endif