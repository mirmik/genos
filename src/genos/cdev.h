#ifndef GENOS_CHAR_DEVICE_H
#define GENOS_CHAR_DEVICE_H

#include <genos/dev.h>
#include <defs/ioflags.h>

namespace genos 
{
	class char_device : public genos::named_node
	{
	public:
		char_device(const char* name) : 
			named_node(name) 
		{
			devmngr.link(this);
		} 
	};
}

#endif