#ifndef GENOS_MVFS_OPSTYPES_H
#define GENOS_MVFS_OPSTYPES_H

typedef int(*open_t)(struct file*);
typedef int(*release_t)(struct file*);
typedef int(*write_t)(struct file*, const char* data, unsigned int size);
typedef int(*read_t)(struct file*, char* data, unsigned int size);
typedef int(*iterate_t)(struct file*, void*);

//inode operations
typedef int(*mkdir_t)(const char * name, size_t nlen, int flags);
typedef int(*rmdir_t)();


#endif