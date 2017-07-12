#ifndef KERNEL_NAMESPACE_OPERATIONS_H
#define KERNEL_NAMESPACE_OPERATIONS_H

#define Error_WrongFD -2
#define Error_WrongPath -3
#define Error_WrongDir -4
#define Error_IsDir -5
#define Error_IsNoDir -6

#include <kernel/fs/file.h>
#include <kernel/fs/inode.h>
#include <kernel/fs/dirent.h>
#include <datastruct/argvc.h>

namespace Kernel {

	Inode* rootLookup(const char* path);
	Inode* rootLookup(argvc_t&);

	int open(const char* path, int mode);
	int write(int fd, const char* data, int length);	
	Kernel::dirent* readdir(int fd, char* buf, int lmax);

	int mkdir(const char* path);
	int mkchardev(const char* path, Kernel::CharDev* dev);
}

#endif