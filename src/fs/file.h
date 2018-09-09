#ifndef GENOS_FS_FILE_H
#define GENOS_FS_FILE_H

struct inode;
struct file;

/**
 *	В эти операции транслируются системные вызовы.
 */
struct file_operations {
	int (*open) (struct inode*, struct file*);
	int (*release) (struct inode*, struct file*);
	int (*write)(struct file *file, const char *data, unsigned int size);
	int (*read)(struct file *file, char *data, unsigned int size);
};

/**
 *	Данная структура представляет открытый файл в пространстве ядра.
 */
struct file {
	const struct file_operations* fs_ops;
	void* privdata;
};

#endif