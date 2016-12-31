#ifndef GENOS_FILESYSTEM_INODE_H
#define GENOS_FILESYSTEM_INODE_H

#include <kernel/fs/chardev.h>

namespace Kernel {

	class File;

	class Inode {
	public:		
		virtual File* getFile() = 0;

		virtual bool is_chardev() = 0;
		virtual bool is_blockdev() = 0;	
		virtual bool is_dir() = 0;

		virtual int mkdir(const char* path) {
			return -1;
		};

		virtual int mkchardev(const char* path, Kernel::CharDev* dev) {
			return -1;
		};	
	};

}

#endif