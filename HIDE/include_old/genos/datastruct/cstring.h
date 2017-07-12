#ifndef GENOS_CSTRING_H
#define GENOS_CSTRING_H

#include "genos/datastruct/dynbuf.h"

struct cstring 
{
	struct dynbuf db;
	size_t len;
};

static inline void cstring_init(struct cstring* cs)
{
	dynbuf_init(&cs->db);
	cs->len = 0;
};

static inline void cstring_assign_cstr(struct cstring* cs, 
	allocator_ops* alloc, const char* cstr)
{
	size_t newlen = strlen(cstr);
	dynbuf_reserve(&cs->db, alloc, newlen + 1);
	strcpy((char*)cs->db.buf, cstr);
	cs->len = newlen;
};

static inline void cstring_concat_cstr(struct cstring* cs, 
	allocator_ops* alloc, const char* cstr)
{
	size_t newlen = cs->len + strlen(cstr);
	dynbuf_reserve(&cs->db, alloc, newlen + 1);
	strcpy((char*)cs->db.buf + cs->len, cstr);
	cs->len = newlen;
};

static inline char* cstring_cstr(struct cstring* cs)
{
	return (char*) cs->db.buf;
};

static inline void cstring_invalidate(struct cstring* cs,
	allocator_ops* alloc)
{
	dynbuf_invalidate(&cs->db,alloc);
	cs->len = 0;
};

#endif