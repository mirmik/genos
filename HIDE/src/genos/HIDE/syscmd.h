#ifndef GENOS_SYSCMD_H
#define GENOS_SYSCMD_H

#include <igris/shell/conscmd.h>
#include <igris/shell/executor.h>

namespace genos 
{
	void set_system_executor(igris::executor * executor);
	int system(const char * cmd);

	extern igris::executor * system_executor;
}

#endif