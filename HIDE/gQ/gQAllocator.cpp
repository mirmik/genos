#include <gQ/gQAllocator.h>

/*
void* operator new(size_t sz, gQAllocator* alloc) {
	return alloc->allocate(sz);
}

void operator delete(void* ptr, gQAllocator* alloc) {
	alloc->deallocate(ptr);
}
*/