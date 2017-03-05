#ifndef GENOS_KERNEL_QUERY_H
#define GENOS_KERNEL_QUERY_H

#include <kernel/id/id.h>

struct query {
public:
	dlist_head lnk;
	
	ipcstack *stack;
	int16_t errstat;

	qid_t sender;
	qid_t receiver;
};

#endif