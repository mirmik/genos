#ifndef GXX_BASE_H
#define GXX_BASE_H

#include <inttypes.h>
//#include <gxx/Allocator.h>

#ifdef GENOS_CONFIGURE
#	include "genos_configure.h"
#endif

#ifndef GXX_DEFAULT_SEED
#	define GXX_DEFAULT_SEED 0xABCDEFAB
#endif

#ifndef GXX_DEFAULT_ALLOCATOR
#	define GXX_DEFAULT_ALLOCATOR &mallocAllocator 
#endif

namespace gxx {

	//MallocAllocator mallocAllocator;
	//CtrlMallocAllocator ctrlmallocAllocator;	
	//Allocator* defaultAllocator = GXX_DEFAULT_ALLOCATOR;

	//uint32_t defaultSeed = GXX_DEFAULT_SEED;

};


#endif