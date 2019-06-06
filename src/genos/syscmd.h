#ifndef GENOS_SYSCMD_H
#define GENOS_SYSCMD_H

#include <igris/datastruct/dlist.h>

#define SH_INTERNAL_SPLIT 0x01

#define CMDFUNC 0
#define CMDAUTOM 1
#define CMDCOOP 2

#define SYSCMD_OK 0

#define SYSCMD_TBLFIN {NULL,NULL,0,NULL}

namespace genos 
{
	using syscmd_func_t = int (*)(int, char**);

	struct syscmd_command 
	{
		const char* name;
		void * func;
		uint8_t type;
		const char* help;

		constexpr syscmd_command(
			const char* name,
			int (*func)(int, char**),
			uint8_t type,
			const char* help) 
		:
			name(name),
			func((void*)func),
			type(type),
			help(help)
		{}		
	};
}

#define SYSCMD(name, ...) \
genos::syscmd_command name[] {__VA_ARGS__, SYSCMD_TBLFIN}

#endif