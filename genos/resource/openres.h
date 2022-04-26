#ifndef GENOS_RESOURCE_OPENRES_H
#define GENOS_RESOURCE_OPENRES_H

#include <igris/compiler.h>

struct openres 
{
	struct file_head * file;
	int pos;
};

__BEGIN_DECLS

void openres_init(struct openres * ores, struct file_head * file);
int current_schedee_get_free_openres(struct openres ** ores);

__END_DECLS

#endif