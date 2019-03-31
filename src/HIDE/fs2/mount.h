#ifndef GENOS_FS_MOUNT_H
#define GENOS_FS_MOUNT_H

#include <sys/cdefs.h>
#include <igris/datastruct/dlist.h>

struct inode;
struct path;

//#define MOUNT_DESC_STRINFO_LEN 16
struct mount {
//	struct node *mnt_point;
//	struct node *mnt_root;
///	struct mount_descriptor *mnt_parent;
//	struct dlist_head mnt_mounts;
//	struct dlist_head mnt_child;
//	char mnt_dev[MOUNT_DESC_STRINFO_LEN];
};

__BEGIN_DECLS

struct mount *mount_table(void);

//extern struct mount_descriptor *mount_table_add(struct path *mnt_point_path,
//		struct node *root, const char *dev);

//extern int mount_table_del(struct mount_descriptor *mdesc);

//extern struct mount_descriptor *mount_table_get_child(struct mount_descriptor *parent, struct node *mnt_point);

//extern int mount(char *dev,  char *dir, char *fs_type);

//extern int umount(char *dir);

__END_DECLS

#endif 
