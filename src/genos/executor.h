#ifndef GENOS_EXECUTOR_H
#define GENOS_EXECUTOR_H

#include <genos/syscmd.h>

#define EXECUTOR_PROCESS_STARTED 2
#define EXECUTOR_ERROR -1

#define EXECUTOR_TBLFIN 

namespace genos 
{
	class executor 
	{
		syscmd_command ** tbl;

	public:
		executor(genos::syscmd_command** syscmdtbl) : tbl(syscmdtbl) {}

		virtual int execute(char * line, size_t len);
	};
}

#define EXECUTOR(name, ...) \
genos::syscmd_command* name##_syscmd_table[] {__VA_ARGS__, EXECUTOR_TBLFIN}; \
genos::executor name{name##_syscmd_table}

#endif