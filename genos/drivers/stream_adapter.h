#ifndef GENOS_DRIVERS_STREAM_PIPE_H
#define GENOS_DRIVERS_STREAM_PIPE_H

#include <genos/drivers/device_head.h>
#include <genos/tasklet.h>

struct stream_adapter_operations 
{
	int (*avail)(void * priv);
	int (*room)(void * priv);
	int (*write)(void * priv, const char * data, unsigned int size);
	int (*read)(void * priv, char * data, unsigned int size);
};

struct stream_adapter 
{
	struct device_head dev;

	void * priv;
	const struct stream_adapter_operations * stream_ops; 

	struct dlist_head wqueue;
	struct dlist_head rqueue;
	struct tasklet checker;
};

__BEGIN_DECLS

void stream_adapter_init(
	struct stream_adapter * adapter,
	const struct stream_adapter_operations * ops
);

__END_DECLS

#endif