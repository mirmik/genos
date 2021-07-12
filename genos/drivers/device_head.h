#ifndef GENOS_DEVICE_HEAD_H
#define GENOS_DEVICE_HEAD_H

#include <igris/compiler.h>
#include <igris/datastruct/dlist.h>
#include <genos/resource/file_head.h>

#define DEVICE_NAME_MAXLEN 8

struct device_head 
{
	struct file_head fil; 

	char name[DEVICE_NAME_MAXLEN];
	struct dlist_head lnk_devices_list;
};

extern struct dlist_head devices_list;

__BEGIN_DECLS

void device_head_init(struct device_head * dev, const char * name, 
	const struct file_operations * f_ops);

__END_DECLS

#endif