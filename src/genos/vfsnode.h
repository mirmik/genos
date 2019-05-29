#ifndef GENOS_TREENODE_H
#define GENOS_TREENODE_H

#include <genos/resource.h>
#include <igris/datastruct/dlist.h>

#define TREEPART_OPENABLE 0x01
#define TREEPART_PARENT 0x02

namespace genos
{
	class vfsnode
	{
	public:
		vfsnode * parent;
		dlist_head lnk;
		uint8_t vfsnode_type;

		vfsnode(uint8_t vfsnode_type) : vfsnode_type(vfsnode_type) {}

		bool is_openable()
		{ return vfsnode_type & TREEPART_OPENABLE; }
	};

	class vfsnode_resource : public vfsnode, public resource
	{
	public:
		vfsnode_resource(uint8_t type = 0) : vfsnode(type | TREEPART_OPENABLE) {}
	};

	class vfsnode_stream_resource : public vfsnode, public stream_resource
	{
	public:
		vfsnode_stream_resource(uint8_t type = 0) : vfsnode(type | TREEPART_OPENABLE) {}
	};

	class vfsnode_directory : public vfsnode
	{
	public:
		dlist_head childs;

		vfsnode_directory(uint8_t type = 0) : vfsnode(type | TREEPART_PARENT) {}
		virtual int iterate();
		virtual int lookup(vfsnode** tnode);
	};
}

#endif