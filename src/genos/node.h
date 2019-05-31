#ifndef GENOS_NODE_H
#define GENOS_NODE_H

#include <igris/datastruct/dlist.h>
#include <errno.h>
#include <string.h>
#include <genos/ctrobj.h>

#define GENOS_RESOURCE_FILE 1
#define GENOS_RESOURCE_DIRECTORY 1

namespace genos 
{
	class opennode;

	struct node 
	{
	public:
		virtual int write(const void* data, size_t size, int flags) { return ENOTSUP; }
		virtual int read(void* data, size_t size, int flags) { return ENOTSUP; }

		//virtual int write(const void* data, size_t size, genos::opennode* onode) { return ENOTSUP; }
		//virtual int read(void* data, size_t size, genos::opennode* onode) { return ENOTSUP; }

		virtual int release() { return 0; }
		virtual int open(genos::opennode * filp) { return 0; }
	};

	class directory 
	{
	public:
		virtual int iterate(char* buffer, size_t maxsz, genos::opennode* onode) { return ENOTSUP; }
		virtual int mknode(const char* childname, genos::opennode* onode) { return ENOTSUP; }
		virtual int rmnode(const char* childname, genos::opennode* onode) { return ENOTSUP; }
	};

	class opennode 
	{
	public:
		union {
			uintptr_t pos;
			void * ptr;
			dlist_head * dh; 
		};
		
		uint8_t restype;

		int flags;

		union 
		{
			genos::node * node;
			genos::directory * dir; 
		};
	};

	class named_node : public node
	{
	public:
		dlist_head lnk;
		const char* name;

	public:
		named_node(){}
		named_node(const char* name) : lnk(DLIST_HEAD_INIT(lnk)), name(name) {}
	};

	class named_node_list : public directory
	{
	public:
		dlist_head list = DLIST_HEAD_INIT(list);

		int iterate(char* buffer, size_t maxsz, genos::opennode* onode) override 
		{ 
			if (onode->dh == nullptr) 
				onode->dh = list.next;	
			else 
				onode->dh = onode->dh->next;
			
			if (onode->dh == &list) { 
				buffer[0] = 0;
				return 0; 
			}
			
			strncpy(buffer, dlist_entry(onode->dh, named_node, lnk)->name, maxsz);
			return 0;
		}
	};

	int open_node(genos::node * res);
	int open_directory(genos::directory * res);
}

#endif