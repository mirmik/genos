#ifndef GENOS_ALLOCATOR_OPS
#define GENOS_ALLOCATOR_OPS

#include "defines/size_t.h"
#include <sys/cdefs.h>

__BEGIN_DECLS

struct allocator_operations {
	void* (*allocate) (size_t);
	void (*deallocate) (void*);
	void* (*reallocate) (void*, size_t sz);
};

typedef struct allocator_operations alloc_ops;

extern struct allocator_operations malloc_ops;
extern struct allocator_operations ctrl_malloc_ops;


void* ctrlmalloc (size_t sz);
void ctrlfree (void* ptr);
void* ctrlrealloc (void* ptr, size_t sz);

__END_DECLS

#include "stdlib.h"

//#define objalloc malloc
//#define objfree free

#endif