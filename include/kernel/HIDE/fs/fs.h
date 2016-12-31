#ifndef GENOS_FILESYSTEM_H
#define GENOS_FILESYSTEM_H

#include <gxx/DList.h>
#include <gxx/HashTable.h>
#include <string.h>

class Inode;
class SuperBlock;
class FileSystem;

enum FType {
	type_file,
	type_dir,
	type_chardev,
	type_blkdev,
	type_socket,
};

class Inode 
{
public:
	FType	 	type;
	uint16_t 	nlink;
};

class SuperBlock 
{
public:
	dlist_head 	lnk;
	
	Inode* getRoot();

	SuperBlock() {
	}

	//virtual Inode* inode_allocate();
};

class FileSystem 
{
public:
	gxx::DList<SuperBlock, &SuperBlock::lnk> sb_list;
	
	virtual SuperBlock* getsb(void* data) = 0;
};

#endif