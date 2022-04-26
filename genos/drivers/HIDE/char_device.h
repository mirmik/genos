#ifndef GENOS_CHAR_DEVICE_H
#define GENOS_CHAR_DEVICE_H

#include <igris/compiler.h>
#include <genos/drivers/device.h>

struct char_device 
{
	struct device_head dev;
};

__BEGIN_DECLS

void char_device_init() 
{
	
}

__END_DECLS

#endif
