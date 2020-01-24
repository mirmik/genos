#ifndef GENOS_SYSCMD_H
#define GENOS_SYSCMD_H

#include <igris/shell/conscmd.h>
#include <utility>

#warning "genos syscmd is deprecated. use igris conscmd instead"

namespace genos 
{
	using syscmd_func_t = int (*)(int, char**);

	struct system_command : public igris::console_command
	{
		template<class ... Args>
		system_command(Args&& ... args) : 		
			igris::console_command(std::forward<Args>(args) ...) 
		{}
	};
}

#endif