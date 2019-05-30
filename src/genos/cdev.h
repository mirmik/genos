#ifndef GENOS_CHAR_DEVICE_H
#define GENOS_CHAR_DEVICE_H

#include <genos/resource.h>
#include <genos/vfs.h>

#include <defs/ioflags.h>

namespace genos 
{
	class nolinked_char_device : public stream_resource 
	{

	};

	class char_device : public vfsnode_stream_resource
	{
	public:
		char_device(const char* name) : vfsnode_stream_resource(name) {} 
	};
}

#endif