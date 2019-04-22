#ifndef MVFS_NODESTUB_H
#define MVFS_NODESTUB_H

#include <igris/datastruct/dlist.h>

struct nodestub
{
	struct dlist_head txwlst;
	struct dlist_head rxwlst;
};

#define NODESTUB_INIT(name) 		\
{ DLIST_HEAD_INIT(name.txwlst), DLIST_HEAD_INIT(name.rxwlst) }

#endif