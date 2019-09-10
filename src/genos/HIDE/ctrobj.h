#ifndef GENOS_CTROBJ_H
#define GENOS_CTROBJ_H

#include <stdint.h>
#include <igris/datastruct/dlist.h>

#define CTROBJ_SCHEDEE_LIST 0
#define CTROBJ_WAITER_SCHEDEE 1
#define CTROBJ_WAITER_DELEGATE 2
#define CTROBJ_KTIMER_SCHEDEE 3
#define CTROBJ_KTIMER_DELEGATE 4

struct ctrobj 
{
	struct dlist_head lnk;
	uint8_t type;
};

#define CTROBJ_DECLARE(name, type) \
	{ DLIST_HEAD_INIT(name.lnk), type } 

__BEGIN_DECLS

static inline
void ctrobj_init(struct ctrobj * obj, uint8_t type) 
{
	obj -> type = type;
	dlist_init(& obj->lnk);
} 

__END_DECLS

#endif