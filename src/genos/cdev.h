#ifndef GENOS_CHAR_DEVICE_H
#define GENOS_CHAR_DEVICE_H

#include <genos/dev.h>
#include <defs/ioflags.h>

#include <igris/util/numconvert.h>

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

		int print(const char * str, int flags = 0) 
		{ 
			return write(str, strlen(str), flags); 
		}

		int print(int32_t arg, int flags) 
		{ 
			char buf[32];
			char* eptr = i32toa(arg, buf, 10);
			return write(buf, eptr - buf, flags); 
		}

		template<typename T>
		int println(const T& arg, int flags) 
		{
			int ret = 0;
			ret += print(arg,flags);
			ret += print("\r\n",flags);
			return ret;
		}	
	};
}

#endif