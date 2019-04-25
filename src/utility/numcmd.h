#ifndef GENOS_UTILITY_NUMCMD_AUTOMATE_H
#define GENOS_UTILITY_NUMCMD_AUTOMATE_H

#include <sys/cdefs.h>
#include <igris/protocols/numcmd/numcmd.h>
#include <igris/datastruct/sline.h>

#include <stdint.h>

#define NUMCMD_LINE_LENGTH 64

struct numcmd_context 
{
	struct numcmd_record * numcmd_table;

	struct sline line;
	struct char_device * cdev;
	char last;
	char buffer[NUMCMD_LINE_LENGTH];
	unsigned char debug_mode;
};

#define NUMCMD_CONTEXT(name, table, cdev) \
struct numcmd_context name = { table, {}, cdev } 

__BEGIN_DECLS

void* numcmd_automate(void* arg, int* state);

__END_DECLS

#endif