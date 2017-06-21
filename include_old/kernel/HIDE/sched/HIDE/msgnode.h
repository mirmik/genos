#ifndef GENOS_VIRTSCHEDEE_H
#define GENOS_VIRTSCHEDEE_H

#include "kernel/id/id.h"

#define CANT_RECEIVE -1
#define WAIT_REPLY 1
#define INSTANT_REPLY 2

struct query {
	struct dlist_head lnk;
	id_t sender;
	void* smsg;
	void* rmsg;
	size_t slen;
	size_t rlen;
};

struct msgnode_operations {
	int8_t(*newquery)(struct query* q);
};

struct msgnode {
	id_t id;
	struct msgnode_operations mops;
};

#endif