#ifndef GENOS_ALLOC_H
#define GENOS_ALLOC_H

#include "genos_configure.h" /* for default_allocator */

#include <stdlib.h>
#include <stdio.h>

struct alloc_t;
typedef struct alloc_t alloc_t;

struct alloc_ops {
	void* 	(*allocate) 	(alloc_t * alloc, size_t sz);
	void* 	(*reallocate)	(alloc_t * alloc, void* data, size_t sz);
	void	(*deallocate)	(alloc_t * alloc, void* data);
};

struct alloc_t {
	struct alloc_ops ops;
};

extern alloc_t allocator_malloc;
extern alloc_t allocator_ctrlmalloc;

static void * malloc_method(alloc_t * alloc, size_t sz) {
	(void) alloc;
	return malloc(sz);
}

static void * realloc_method(alloc_t * alloc, void * ptr, size_t sz) {
	(void) alloc;
	return realloc(ptr, sz);
}

static void free_method(alloc_t * alloc, void * ptr) {
	(void) alloc;
	free(ptr);
}

static void * ctrlmalloc_method(alloc_t * alloc, size_t sz) {
	(void) alloc;
	printf("malloc %d\r\n", sz);
	return malloc(sz);
}

static void * ctrlrealloc_method(alloc_t * alloc, void * ptr, size_t sz) {
	(void) alloc;
	printf("realloc %d\r\n", sz);
	return realloc(ptr, sz);
}

static void ctrlfree_method(alloc_t * alloc, void * ptr) {
	(void) alloc;
	printf("free \r\n");
	free(ptr);
}
#endif