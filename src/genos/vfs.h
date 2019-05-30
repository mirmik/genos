#ifndef GENOS_TREENODE_H
#define GENOS_TREENODE_H

#include <genos/resource.h>
#include <igris/datastruct/dlist.h>

#define TREEPART_OPENABLE 0x01
#define TREEPART_PARENT 0x02

namespace genos
{
	class vfsnode_resource;

	class vfsnode
	{
	public:
		vfsnode * parent;
		dlist_head lnk;

		const char * name;
		uint8_t vfsnode_type;

		vfsnode(const char * name, uint8_t vfsnode_type) 
			: name(name), vfsnode_type(vfsnode_type) 
		{}

		int open(opened_resource * ores) 
		{
			(vfsnode_resource)
		}
	};

	class vfsnode_resource : public vfsnode, public resource
	{
		
	};

	class vfsnode_stream_resource : public vfsnode, public stream_resource
	{
	public:
		vfsnode_stream_resource(const char * name, uint8_t type = 0) 
		: vfsnode(name, type | TREEPART_OPENABLE) {}
	};

	class directory : public vfsnode
	{
	public:
		dlist_head childs;

		directory(const char* name, uint8_t type = 0) : 
			vfsnode(name, type | TREEPART_PARENT), childs(DLIST_HEAD_INIT(childs)) {}
		
		virtual int iterate() { return ENOENT; }
		virtual int lookup(vfsnode** tnode) { return ENOENT; }

		void add_child(vfsnode* node) 
		{
			dlist_add(&node->lnk, &childs);
		}
	};

	extern directory root_directory;
}

#endif