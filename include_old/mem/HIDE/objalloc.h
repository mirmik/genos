#ifndef GENOS_OBJALLOC_H
#define GENOS_OBJALLOC_H

#include "stdlib.h"

struct objallocator {
	void* (allocate) (struct objallocator*);
	void (deallocate) (struct objallocator*, void*);
};

/*static void* objmalloc(struct objallocator* _alloc)
{
	struct objalloc_malloc* alloc = (struct objalloc_malloc*)_alloc;
	return malloc(alloc->sz);
}

static void objfree(struct objallocator* _alloc, void* ptr)
{
	free(ptr);
}

struct objalloc_malloc
{
	struct objallocator ops;
	size_t sz;
};

struct objallocator* construct_obj_allocator(int sz)
{

};*/

#endif