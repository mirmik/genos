#ifndef DATASTRUCT_DYNARRAY_H
#define DATASTRUCT_DYNARRAY_H

#include <util/math.h>

#include <assert.h>
#include <sys/cdefs.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <mem/alloc.h>

typedef struct {
	void* data;
	size_t cap;
	alloc_t * alloc;
} dynarray_t;

__BEGIN_DECLS

static inline void dynarray_init(dynarray_t * da) {
	da->data = NULL;
	da->cap  = 0;
	da->alloc = default_allocator;
}

static inline void dynarray_invalidate(dynarray_t * da) {
	if (da->data) da->alloc->ops.deallocate(da->alloc, da->data);
	da->data = NULL;
	da->cap  = 0;
}

static inline void dynarray_setAllocator(dynarray_t * da, alloc_t * alloc) {
	da->alloc = alloc;
};

static inline int8_t dynarray_setSize(dynarray_t * da, size_t size) {
	if (size == 0) dynarray_invalidate(da);
	
	da->data = da->alloc->ops.reallocate(da->alloc, da->data, size);
	if (da->data == NULL) return 0;
	
	da->cap  = size;
	return 1;
}

static inline int8_t dynarray_reserve(dynarray_t * da, size_t size) {
	return size <= da->cap  ? 1 : dynarray_setSize(da, size);	
}

/* 
	Paste piece into dynarray.

	Before: fffffsssss
	After:	ffffftttttsssss

	f - first piece
	s - second piece
	t - paste
*/
static inline void* dynarray_paste (dynarray_t * da, size_t biaspos, size_t biaslen) {
	size_t tocopy = da->cap  + biaslen;
	size_t newsize = tocopy;
	
	void* newdata = da->alloc->ops.allocate(da->alloc, newsize);
	if (newdata == NULL) return NULL;
	
	memcpy(newdata, da->data, biaspos);
	memcpy((char*)newdata + biaspos + biaslen, (char*)da->data + biaspos, tocopy - biaspos);
	
	if (da->data) da->alloc->ops.deallocate(da->alloc, da->data);
	da->data = newdata;
	da->cap  = newsize;
	
	return (char*)da->data + biaspos;
}

/* 
	Delete piece from dynarray.

	Before: fffffrrrrrsssss
	After:	fffffsssss

	f - first piece
	s - second piece
	r - removed piece 
*/
static inline void dynarray_remove(dynarray_t * da, size_t biaspos, size_t biaslen) {
	size_t newsize = da->cap  - biaslen;

	memmove((char*)da->data + biaspos, (char*)da->data + biaspos + biaslen, biaslen);
	da->data = da->alloc->ops.reallocate(da->alloc, da->data, newsize);

	da->cap  = newsize;

	return;
}

static inline size_t dynarray_memcpy(dynarray_t * da, const char* str, size_t len) {
	dynarray_setSize(da, len);
	memcpy(da->data, str, len);

	return len;
}

static inline size_t dynarray_mempaste(dynarray_t * da, size_t pos, const char* str, size_t len) {
	if (pos > da->cap ) return 0;

	void* ptr = dynarray_paste(da, pos, len);
	memcpy(ptr, str, len);

	return len;
}

static inline size_t dynarray_strcpy(dynarray_t * da, const char* str) {
	return dynarray_memcpy(da, str, strlen(str));
}

static inline size_t dynarray_strpaste(dynarray_t * da, size_t pos, const char* str) {
	return dynarray_mempaste(da, pos, str, strlen(str));
}

__END_DECLS

#define dynarray_ref(da,num) (*((char*)(da)->data + num))
#define dynarray_byteArray(da) ((char*)(da)->data)

#endif