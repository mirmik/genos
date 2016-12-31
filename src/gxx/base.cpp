#ifndef GXX_BASE_H
#define GXX_BASE_H

#include <inttypes.h>
#include <gxx/Allocator.h>
#include "genos_configure.h"

namespace gxx {

	MallocAllocator mallocAllocator;
	CtrlMallocAllocator ctrlmallocAllocator;	
	Allocator* defaultAllocator = GXX_DEFAULT_ALLOCATOR;

	uint32_t defaultSeed = GXX_DEFAULT_SEED;

};


#endif