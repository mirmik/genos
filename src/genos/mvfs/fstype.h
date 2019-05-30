#ifndef GENOS_MVFS_FSTYPE_H
#define GENOS_MVFS_FSTYPE_H

namespace genos 
{
	struct slist_head lnk;
	const char * name;
	const struct file_system_operations * fs_ops;

	virtual struct super_block * get_sb(struct file_system_type*, int, const void *) = 0;	
	virtual void kill_sb(struct file_system_type*, struct super_block *) = 0;
}

#endif