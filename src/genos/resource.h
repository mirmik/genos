#ifndef GENOS_RESOURCE_H
#define GENOS_RESOURCE_H

#include <string.h>
#include <errno.h>

#include <igris/datastruct/dlist.h>
#include <igris/dprint.h>
#include <igris/math.h>
#include <igris/buffer.h>

#include <limits.h>

#define GENOS_RESOURCE_FILE 1
#define GENOS_RESOURCE_DIRECTORY 2

namespace genos
{
	class openres;

	class resource
	{
	public:
		virtual int release() { return ENOTSUP; }
		virtual int open(openres * ores) { return ENOTSUP; }
		virtual int gettype() { return 0; }
	};


	struct node : public resource
	{
	public:
		virtual int write(const void* data, size_t size, int flags) { errno = ENOTSUP; return -1; }
		virtual int read(void* data, size_t size, int flags) { errno = ENOTSUP; return -1; }
		virtual size_t room() { return INT_MAX; }
		virtual size_t avail() { return INT_MAX; }
		//virtual int write(const void* data, size_t size, genos::openres* onode) { return ENOTSUP; }
		//virtual int read(void* data, size_t size, genos::openres* onode) { return ENOTSUP; }

		int print(const char * data, int flags=0);
		int println(const char * data, int flags=0);
		int readline(char * data, int flags=0);

		int gettype() override { return GENOS_RESOURCE_FILE; }
	};

//	int open_node(genos::node * res, genos::openres * ores);
//	int open_node(genos::node * res, int flags);
	int open_resource(genos::resource * res, genos::openres * ores, int flags);
	int open_resource(genos::resource * res, int16_t flags);


	class directory : public resource
	{
	public:
		virtual int iterate(char* buffer, size_t maxsz, genos::openres* onode) { return ENOTSUP; }
		virtual int mknode(const char* childname, genos::openres* onode) { return ENOTSUP; }
		virtual int rmnode(const char* childname, genos::openres* onode) { return ENOTSUP; }

		int gettype() override { return GENOS_RESOURCE_DIRECTORY; }
	};

	int open_directory(genos::directory * res);
	int open_directory(genos::directory * res, genos::openres * ores);


	class openres
	{
	public:
		union
		{
			genos::resource * res;
			genos::node * node;
			genos::directory * dir;
		};
		int16_t flags;

		void close()
		{
			node->release();
			node = nullptr;
		}
	};

	struct restbl
	{
		genos::openres * tbl = nullptr;
		int tblsize = 0;
		int refs = 0;
		void(*deleter)(restbl*) = nullptr;

	public:
		restbl() {}

		int size()
		{
			return tblsize;
		}

		void up()
		{
			refs++;
		}

		void down()
		{
			refs--;

			if (refs == 0)
			{
				for (int i = 0; i < tblsize; ++i)
				{
					tbl[i].close();
				}
				tblsize = 0;
			}
		}

		genos::openres * operator[](int i)
		{
			assert(tbl);
			assert(tblsize > i);

			if (i >= tblsize)
				return nullptr;

			return &tbl[i];
		}

		void attach_restbl(genos::openres * _tbl, int _tblsz)
		{
			tbl = _tbl;
			tblsize = _tblsz;
			memset(tbl, 0, sizeof(genos::openres) * tblsize);
		}

		int get_available_fd();
	};


	class named_node : public node
	{
	public:
		dlist_head lnk;
		const char* name;

	public:
		named_node() {}
		named_node(const char* name) : lnk(DLIST_HEAD_INIT(lnk)), name(name) {}
	};

	class named_node_list : public directory
	{
	public:
		dlist_head list = DLIST_HEAD_INIT(list);

		int iterate(char* buffer, size_t maxsz, genos::openres* onode) override
		{
			/*	if (onode->dh == nullptr)
					onode->dh = list.next;
				else
					onode->dh = onode->dh->next;

				if (onode->dh == &list)
				{
					buffer[0] = 0;
					return 0;
				}

				strncpy(buffer, dlist_entry(onode->dh, named_node, lnk)->name, maxsz);
			*/	return 0;
		}
	};

	class buffer_node : public genos::node
	{
	private:
		char * buf;
		size_t cap = 0;
		size_t cursor = 0;

	public:
		buffer_node(char* buf, int size)
			: buf(buf), cap(size)
		{}

		int write(const void* data, size_t size, int flags) override
		{
			size_t sz = MIN(room(), size);
			memcpy(&buf[cursor], data, sz);
			cursor += sz;
			return sz;
		}

		size_t room() override
		{
			return cap - cursor;
		}

		igris::buffer buffer() { return { buf, cursor }; }
		void reset() { cursor = 0; }
	};
}

#endif