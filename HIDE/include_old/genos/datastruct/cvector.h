#ifndef CVECTOR_H
#define CVECTOR_H

#include "assert.h"
#include "mem/allocator.h"
#include "defines/size_t.h"
#include <sys/cdefs.h>

struct cvector 
{
	void* buffer;
	size_t capacity;
	size_t length;

	allocator_ops* alloc;
};

__BEGIN_DECLS

inline void cvector_init(struct cvector* cv, allocator_ops* alloc)
{
	cv->buffer = 0;
	cv->capacity = 0;
	cv->length = 0;
	cv->alloc = alloc;
}

inline void cvector_change_buffer(struct cvector* cv, size_t sz)
{
	if (cv->buffer) cv->buffer = cv->alloc->reallocate(cv->buffer, sz);
	else cv->buffer = cv->alloc->allocate(sz);
	cv->capacity = sz;
}

inline void cvector_reserve(struct cvector* cv, size_t sz)
{
	if (cv->capacity < sz) cvector_change_buffer(cv, sz);
}

inline void cvector_invalidate(struct cvector* cv)
{
	if (cv->buffer) cv->alloc->deallocate(cv->buffer);
	cv->buffer = 0;
	cv->capacity = 0;
	cv->length = 0;
}

inline void cvector_shift(struct cvector* cv, 
	size_t spos, size_t bias)
{
	char* buf = (char*) cv->buffer;
	assert(spos <= cv->length);

	size_t newl = cv->length + bias;

	cvector_reserve(cv, newl);

	memmove(buf+spos+bias, buf+spos, cv->length - spos);
	cv->length = newl;
};

inline void* __cvector_push_back(struct cvector* cv, size_t sz)
{
	size_t oldl = cv->length;
	size_t newl = oldl + sz;
	cvector_reserve(cv, newl);
	cv->length = newl;
	return (void*)((char*)cv->buffer + oldl);
};											

inline void* __cvector_push_front(struct cvector* cv, size_t sz)
{
	size_t oldl = cv->length;
	size_t newl = oldl + sz;
	cvector_shift(cv, 0, sz);
	return cv->buffer;
};					

inline void* __cvector_get(struct cvector* cv, size_t bias)
{
	return (void*)((char*)cv->buffer + bias);
};											

#define cvector_push_back(cv,type,val)	\
*(type*)__cvector_push_back(cv,sizeof(type)) = val

#define cvector_push_front(cv,type,val)	\
*(type*)__cvector_push_front(cv,sizeof(type)) = val

#define cvector_get(cv,type,pos)	\
*(type*)__cvector_get(cv,sizeof(type) * pos)

__END_DECLS

#endif