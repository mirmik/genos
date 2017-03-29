#ifndef GENOS_KERNEL_QUERY_H
#define GENOS_KERNEL_QUERY_H

#include <kernel/id/id.h>
#include <datastruct/dlist_head.h>

const MSGTAG_QUERY 0x01
const MSGTAG_ERROR 0x02

struct msgtag {
	struct dlist_head lnk;
	
	struct ipcstack *stack;
	int8_t stat;

	qid_t sender;
	qid_t receiver;
};

typedef struct msqtag msgtag_t;

__BEGIN_DECLS


__END_DECLS

#endif