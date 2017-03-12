#ifndef GENOS_KERNEL_QUERY_H
#define GENOS_KERNEL_QUERY_H

#include <kernel/id/id.h>
#include <datastruct/dlist_head.h>

struct query {
//public:
	struct dlist_head lnk;
	
	struct ipcstack *stack;
	int16_t errstat;

	qid_t sender;
	qid_t receiver;
};

#endif