#ifndef GENOS_SHELL_UTILITY_H
#define GENOS_SHELL_UTILITY_H

#include <sys/cdefs.h>
#include <stdint.h>

#define SH_INTERNAL_SPLIT 0x01

#define MSHELL_FUNCTION 0
#define MSHELL_AUTOMATE 1
#define MSHELL_PROCESS 2

#define MSHELL_OK 0

struct mshell_command
{
	const char* name;
	int (*func) (int, char**);
	uint8_t type;
	const char* help;
};

#define MSHELL_TBLFIN { NULL, NULL, 0, NULL }

//struct mshell_command mshell_command_table[N];

__BEGIN_DECLS

//void * mshell_utility(void * arg, int * state);
int mshell_execute(char* str, int flags, int * retptr);
int mshell_execn(char* str, int len, int flags, int * retptr);

__END_DECLS

#endif