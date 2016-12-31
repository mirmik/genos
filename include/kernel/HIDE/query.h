#ifndef GENOS_QUERY_H
#define GENOS_QUERY_H

#include "genos/datastruct/dlist_head.h"

#define SCHEDEE_QUERY 0
#define NONLOCAL_QUERY 1

struct query {
	dlist_head lnk;
	uint8_t type;
	void* sender;
	void* smsg;
	size_t slen;
	void* rmsg;
	size_t rlen;
};

#endif