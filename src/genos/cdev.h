#ifndef GENOS_CHAR_DEVICE_H
#define GENOS_CHAR_DEVICE_H

#include <genos/node.h>
#include <defs/ioflags.h>

namespace genos 
{
	class char_device : public genos::node
	{
	public:
		char_device(const char* name) : 
			node(name) 
		{} 
	};
}

#endif