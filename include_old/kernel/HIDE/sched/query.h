#ifndef GENOS_QUERY_H
#define GENOS_QUERY_H

#include "compiler.h"
#include "defines/size_t.h"
#include "genos/datastruct/dlist_head.h"
#include "defines/id_t.h"

struct query {
	struct dlist_head lnk;
	id_t sender;
	const void* smsg;
	void* rmsg;
	size_t slen;
	size_t rlen;	//void (*onreply) (struct query*);
};

void automQuery(id_t id, const void* smsg, size_t slen,
		void* rmsg, size_t rlen);
//void __Query(id_t id, struct query* query);
void automReceive(id_t id, void* msg, size_t len);
void automReply(id_t id, const void* msg, size_t len);
id_t QRRret();


#endif