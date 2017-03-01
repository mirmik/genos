#ifndef KERNEL_QUERY_H
#define KERNEL_QUERY_H

#include <datastruct/dlist_head.h>

struct query_t {
	//manage
	struct dlist_head tlnk; // target link
	struct dlist_head slnk; // self link

	//buffers
	void* ibuf;
	size_t ilen;

	void* obuf;
	size_t olen;

	size_t anslen;

	//calback
	void (*callback)(struct query_t*);
	void *private;
}

__BEGIN_DECLS

inline void qreply(struct query_t* query, const void* answer, size_t sz) {
	query->anslen = min(sz, query->olen);
	memcpy(query->obuf, answer, query->anslen);

	dlist_del(tlnk);
	
	if (query->callback) query->callback(query);
}

__END_DECLS

#endif