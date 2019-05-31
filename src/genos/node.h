#ifndef GENOS_NODE_H
#define GENOS_NODE_H

#include <igris/datastruct/dlist.h>
#include <errno.h>

namespace genos 
{
	class node;

	class opennode 
	{
	public:
		int pos;
		int flags;
		uint8_t refs;
		void* priv;
		genos::node * node;
	};

	class node 
	{
		dlist_head lnk;
		const char* name;

	public:
		node(const char* name) : 
			lnk(DLIST_HEAD_INIT(lnk)), name(name) 
		{}

		virtual int write(const void* data, size_t size, int flags) { return ENOTSUP; }
		virtual int read(void* data, size_t size, int flags) { return ENOTSUP; }

		virtual int release() { return 0; }
		virtual int open(genos::opennode * filp) { return 0; }
	};

	int open_node(genos::node * res);
}

#endif