#ifndef GENOS_CHAR_DEVICE_H
#define GENOS_CHAR_DEVICE_H

#include <genos/resource.h>
#include <genos/mvfs/vfsnode.h>

#include <defs/ioflags.h>

namespace genos 
{
	class char_device : public vfsnode
	{
	public:
		char_device(const char* name) : vfsnode(name, 0) {} 
	};
}

#endif