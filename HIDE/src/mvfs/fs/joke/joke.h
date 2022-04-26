#ifndef JOKE_FSTYPE_H
#define JOKE_FSTYPE_H

#include <mvfs/fstype.h>
#include <mvfs/super.h>
#include <mvfs/file.h>
#include <mvfs/dirent.h>

#include <errno.h>
#include <string.h>

struct joke_fstype_class
{
	struct file_system_type fs;
};

struct joke_super_block
{
	struct super_block sb;
};

struct joke_node
{
	struct node node;
};

extern struct joke_fstype_class joke_fstype;
extern struct joke_super_block joke_sb;

#endif