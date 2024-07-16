#ifndef NOS_FAULT_H
#define NOS_FAULT_H

#include <nos/fprint.h>

namespace nos 
{
	template <class ... T>
	static inline void fault(T&& ... args) 
	{
		nos::fprint(std::forward<T>(args)...);
		std::exit(-1);
	}
}

#endif