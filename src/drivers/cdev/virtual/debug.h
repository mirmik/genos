#ifndef GENOS_CHAR_DEBUG_H
#define GENOS_CHAR_DEBUG_H

#include <genos/cdev.h>

namespace genos
{
	class debug_device : public genos::char_device
	{
	public:
		debug_device() : genos::char_device("debug") {}

		int write (const void* data, unsigned int size, int flags) override
		{
			debug_write((char*)data, size);
			return size;
		}

		int read (void* data, unsigned int size, int flags) override
		{
			memset(data, 0, size);
			return size;
		}

		int open(genos::opened_resource * ores) 
		{
			
		}
	};
}

#endif