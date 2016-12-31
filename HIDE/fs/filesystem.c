#include "fs/filesystem.h"
#include <genos/datastruct/slist_head.h>
#include "kernel/panic.h"

#include "string.h"

struct slist_head filesystems_list;

void vfs_init()
{
	slist_init(&filesystems_list);
};

int register_filesystem(struct file_system_type * fs)
{
	if (strchr(fs->name,'.')) panic();
	slist_connect_last(&fs->link, &filesystems_list);
};