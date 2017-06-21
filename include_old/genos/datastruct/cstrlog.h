#ifndef GENOS_CSTRLOG_H
#define GENOS_CSTRLOG_H

#include "inttypes.h"
#include <mem/allocator.h>
#include <compiler.h>

#include "genos/datastruct/dynbuf.h"
#include "genos/datastruct/ring_head.h"
#include "util/array.h"

struct cstrlog
{
	struct dynbuf db;
	struct ring_head rh; 
	alloc_ops* alloc;
};

__BEGIN_DECLS

static inline void cstrlog_init(struct cstrlog* log,
	alloc_ops* alloc, size_t sz)
{
	log->alloc = alloc;
	ring_init(&log->rh, sz);
	dynbuf_init(&log->db);
	dynbuf_reserve(&log->db, alloc, sz * sizeof(struct cstring));
	array_for_each_ptr(ptr, (struct cstring*)log->db.buf, sz)
	{
		cstring_init(ptr);
	}
}

static inline void cstrlog_print(struct cstrlog* log, 
	const char* str)
{
	size_t pos = log->rh.head;
	struct cstring* cstr = &dynbuf_get(&log->db, struct cstring, pos);
	cstring_assign_cstr(cstr, log->alloc, str);
	if (ring_is_full(&log->rh)) {
		ring_move_head_one(&log->rh);
		ring_move_tail_one(&log->rh);
	} else {
		ring_move_head_one(&log->rh);
	};
};

static inline void cstrlog_debug_print_list(struct cstrlog* log)
{	
	ring_for_each(n,&log->rh)
	{
		dprln(
			cstring_cstr(
				&dynbuf_get(&log->db, struct cstring, n)
			)
		);
	};
};

__END_DECLS

#endif