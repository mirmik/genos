#ifndef GENOS_RESOURCE_FILE_HEAD_H
#define GENOS_RESOURCE_FILE_HEAD_H

#include <igris/compiler.h>
#include <stdint.h>

#ifndef O_NONBLOCK
#define O_NONBLOCK 1
#endif

namespace genos 
{
	class file_head 
	{
		uint8_t refs;
		uint8_t flags;

	public:
		bool is_open() { return refs; }
		virtual int write(const void * data, unsigned int size) = 0;
		virtual int read(void * read, unsigned int size) = 0;
		virtual int on_open() = 0;
		virtual int on_release() = 0;
	};
}

#endif