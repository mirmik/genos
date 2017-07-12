#ifndef GENOS_FS_VFS_H
#define GENOS_FS_VFS_H

#include <kernel/fs/fs.h>
#include <kernel/fs/file.h>

class RAMFS_Inode : public Inode 
{
public:
	union {
		File* file;
		//Directory* dir;
	};
};

class RAMFS_SuperBlock : public SuperBlock 
{
public:
	RAMFS_SuperBlock() : SuperBlock() {};

	Inode* inode_allocate() {

	};
};

class RAMFS_FileSystemClass : public FileSystem 
{
public:
	SuperBlock* getsb(void*) 
	{
		RAMFS_SuperBlock* sb = new RAMFS_SuperBlock;
		sb_list.push_back(*sb);
		return sb;
	}
};

extern RAMFS_FileSystemClass RAMFS_FileSystem;

#endif