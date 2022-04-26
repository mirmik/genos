#ifndef GENOS_RESOURCE_RESTBL_H
#define GENOS_RESOURCE_RESTBL_H

#include <genos/resource/openres.h>

struct restbl_head 
{
	struct openres * table;
	int              total;
};

__BEGIN_DECLS
__END_DECLS

#define RESOURCE_TABLE(name, total) \
struct openres name##_table_array[total];  \
struct restbl_head name = { name##_table_array, total };

#endif