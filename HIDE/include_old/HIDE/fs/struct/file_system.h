#ifndef GENOS_FILE_SYSTEM_H
#define GENOS_FILE_SYSTEM_H

#include <genos/datastruct/slist_head.h>

struct file_system_type {
	struct slist_head link;

	const char* name;
	struct dentry *(*mount) (struct file_system_type *, 
		int, const char *, void *);
	void (*umount) (struct super_block *);
};

#endif