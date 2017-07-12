#ifndef GENOS_VID_H
#define GENOS_VID_H

#include "genos/datastruct/hlist_head.h"
#include "defines/id_t.h"
#include "kernel/sched/query.h"

class virtee {
public:
	uint8_t type;
	struct hlist_node hnode;
	id_t pid;
	id_t qret;

	virtual void onquery(query*) = 0;
	virtual void onreceive() = 0;
	virtual void onreply() = 0;

	virtee(){};
};

/*
__BEGIN_DECLS

void virtee_registry(struct virtee* vee);
void virtee_init(struct virtee* vee, struct virtee_operations* ops);

__END_DECLS
*/
#endif