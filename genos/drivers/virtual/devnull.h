#ifndef GENOS_VIRTUAL_DEVNULL_H
#define GENOS_VIRTUAL_DEVNULL_H

#include <genos/drivers/device_head.h>

struct devnull 
{
	struct device_head dev;
};

__BEGIN_DECLS

void devnull_init(struct devnull * devnull);

__END_DECLS

#endif