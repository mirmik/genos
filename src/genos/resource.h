#ifndef GENOS_RESOURCE_H
#define GENOS_RESOURCE_H

#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

#define RESOURCE_STREAM 0x1

namespace genos 
{
	class opened_resource;

	class resource 
	{
	public:
		uint8_t resource_type;

		virtual void release() { return ENOTSUP; }
		virtual int open(opened_resource * ores) { return ENOTSUP; }
	};

	class stream_resource : public resource 
	{
	public:
		virtual int write(const void * data, size_t size, int flags) = 0;
		virtual int read(void * data, size_t size, int flags) = 0;
		virtual int avail() { return 0; }
		virtual int room() { return INT_MAX; }
	};

	class abstract_socket : public stream_resource
	{
	public:
		virtual int connect(void*) = 0;
		virtual int disconnect() = 0;
	};


	class opened_resource 
	{
	public:
		int flags;
		resource * 	res; 
		size_t 		pos;
	};

	class resource_table 
	{
		opened_resource * tbl;
		size_t tblsz;

	public:
		void set_table(opened_resource* tbl, size_t tblsz) 
		{
			this->tbl = tbl;
			this->tblsz = tblsz;
			memset(tbl, 0, sizeof(opened_resource) * tblsz);
		}

		opened_resource& operator[](int fd) { return tbl[fd]; }

		int getfd(int fd, opened_resource ** ores) 
		{
			if (fd >= tblsz || tbl[fd].res == nullptr)
				return EBADF;

			*ores = &tbl[fd];
			return 0;
		}

		int get_available_resource(int * fdp);
	};



	int open_resource(resource* res);
}

#define RESOURCE_TABLE(name, size) genos::opened_resource name[size]

#endif