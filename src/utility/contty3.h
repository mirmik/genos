#ifndef GENOS_UTILITY_CONTTY2_H
#define GENOS_UTILITY_CONTTY2_H

#include <sys/cdefs.h>
#include <igris/shell/readline.h>

#define CONTTY3_LINE_LENGTH 32
#define CONTTY3_HISTORY_SIZE 5

struct contty3_context {
	const char * iopath;
	struct readline rl;	
	unsigned char debug_mode;
	char buffer[CONTTY3_LINE_LENGTH];
	char hbuffer[CONTTY3_LINE_LENGTH * CONTTY3_HISTORY_SIZE];

	int rfd;
	int wfd;
};

__BEGIN_DECLS

void * contty3_automate(void * arg, int * state);
void contty3_debug_mode(struct contty3_context * cntxt, int en);

__END_DECLS

#define CONTTY3_DECLARE(name, iopath) \
struct contty3_context name = { iopath } 


#endif