#ifndef GENOS_UTILITY_CONTTY2_H
#define GENOS_UTILITY_CONTTY2_H

#include <sys/cdefs.h>
#include <igris/shell/readline.h>

#define CONTTY2_LINE_LENGTH 32
#define CONTTY2_HISTORY_SIZE 5

struct serial_device;

struct contty2_context {
	struct readline rl;
	struct serial_device * cdev;
	unsigned char debug_mode;
	char buffer[CONTTY2_LINE_LENGTH];
	char hbuffer[CONTTY2_LINE_LENGTH * CONTTY2_HISTORY_SIZE];
};

__BEGIN_DECLS

void * contty2_automate(void * arg, int * state);
void contty2_debug_mode(struct contty2_context * cntxt, int en);

__END_DECLS

#define CONTTY2_DECLARE(name, cdev) \
struct contty2_context name = { {}, cdev, 0 } 


#endif