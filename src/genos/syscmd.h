#ifndef GENOS_SYSCMD_H
#define GENOS_SYSCMD_H

#include <igris/datastruct/dlist.h>

#define SH_INTERNAL_SPLIT 0x01

#define SYSCMD_FUNCTION 0
#define SYSCMD_AUTOMATE 1
#define SYSCMD_PROCESS 2

#define SYSCMD_OK 0

#define SYSCMD_TBLFIN {NULL,NULL,0,NULL}

namespace genos 
{
	struct shell_command 
	{
		const char* name;
		int (*func) (int, char**);
		uint8_t type;
		const char* help;		
	};

	class syscmd_node
	{
	public:
		dlist_head lnk;
		shell_command * table;
	};
}

#endif