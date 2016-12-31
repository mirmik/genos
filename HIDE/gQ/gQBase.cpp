#include "gQ/gQAllocator.h"
#include "gQ/gQBase.h"
#include "genos_configure.h"

namespace gQ {
	gQMallocAllocator malloc;
	gQCtrlMallocAllocator ctrlmalloc;
};

gQAllocator* gQ::defaultAllocator = GQ_DEFAULT_ALLOCATOR;