#ifndef GENOS_UTILITY_CONTTY_H
#define GENOS_UTILITY_CONTTY_H

#include <sys/cdefs.h>
#include <igris/datastruct/sline.h>

#define CONTTY_LINE_LENGTH 32

struct char_device;

struct contty_context {
	struct sline line;
	struct char_device * cdev;
	char last;
	char buffer[CONTTY_LINE_LENGTH];
	unsigned char debug_mode;
};

__BEGIN_DECLS

void * contty_automate(void * arg, int * state);
void contty_debug_mode(struct contty_context * cntxt, int en);

__END_DECLS

#define CONTTY_CONTEXT(name, cdev) \
struct contty_context name = { {}, cdev } 


#endif