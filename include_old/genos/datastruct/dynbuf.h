#ifndef GENOS_DYNBUF_H
#define GENOS_DYNBUF_H

#include "inttypes.h"
#include <mem/allocator.h>
#include <compiler.h>

struct dynbuf
{
	void* buf;
	size_t cap;
};

__BEGIN_DECLS

static inline int dynbuf_init(struct dynbuf* db)
{
	db->buf = 0;
	db->cap = 0;
};

static inline int dynbuf_invalidate(struct dynbuf* db, alloc_ops* alloc)
{
	if (db->buf) alloc->deallocate(db->buf);
	db->buf = 0;
	db->cap = 0;
	return 0;
};

static inline uint8_t dynbuf_change_buffer(struct dynbuf* db, 
	alloc_ops* alloc, size_t newsz)
{
	void* newbuf = db->buf ? 
		alloc->reallocate(db->buf, newsz) :
		alloc->allocate(newsz);
	if (newbuf) {
		db->buf = newbuf;
		db->cap = newsz;
		return 1;
	}
	return 0;
};

static inline uint8_t dynbuf_reserve(struct dynbuf* db, 
	alloc_ops* alloc, size_t sz)
{
	if (db->buf && db->cap >= sz) return 1;
	if (dynbuf_change_buffer(db,alloc,sz)) {
		return 1;
	}
	return 0;
};

inline void* __dynbuf_get(struct dynbuf* db, size_t bias)
{
	return (void*)((char*)db->buf + bias);
};											

#define dynbuf_get(db,type,pos)	\
*(type*)__dynbuf_get(db,sizeof(type) * pos)


__END_DECLS

#endif