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

__BEGIN_DECLS
__END_DECLS




















/*
struct joke_node * 	joke_node_create(const char * name, size_t nlen);
struct joke_node * 	joke_node_create_as_child(const char * name, size_t nlen, struct node * parent);
void 				joke_node_release(struct joke_node * node);

struct joke_super_block : public super_block {};

struct joke_fstype_class : public file_system_type  
{
	joke_fstype_class() : file_system_type("joke") {};
	struct super_block * get_sb(int flags, const void * data);
	void kill_sb(struct super_block * sb);
};

struct joke_super_block joke_sb;
struct joke_fstype_class joke_fstype;

struct joke_node : public node {
	int mkdir(const char * name, size_t nlen, int flags) {
		struct node * node = joke_node_create_as_child(name, nlen, this);
		node->flag.directory = 1;
		node->sb = &joke_sb;
		return 0;
	}
	
	int rmdir() {	
		dlist_del(&lnk);
		delete this;
		return 0;
	}
	
	int iterate(struct file * filp, void * priv) {
		struct dirent * de = (struct dirent *) priv;
		struct node * parent;
		struct node * d;
	
		if (filp->node->flag.directory == 0) 
			return ENOTSUP;
	
		parent = filp->node;
		
		auto it = parent->childs.begin();

		int n = filp->pos;
		while(n--) it++;
		filp->pos++;
	
		if (it == parent->childs.end()) de->d_name[0] = '\0';
		else {
			de->d_ino = 0;
			strcpy(de->d_name, it->name);
		}
		return 0;
	}
};

struct joke_node * joke_node_create(const char * name, size_t nlen) {
	struct joke_node * ret = new joke_node;
	node_init(ret, name, nlen);
	return ret;
}

struct joke_node * joke_node_create_as_child(const char * name, size_t nlen,
	struct node * parent
) {
	struct joke_node * ret = joke_node_create(name, nlen);
	node_add_child(ret, parent);
	return ret;
}

struct super_block * 
joke_fstype_class::get_sb(int flags, const void * data) 
{
	joke_node * root;
	static uint8_t inited = 0;

	if (inited == 0) {
		inited = 1;
		
		root = joke_node_create("/", 1);
		joke_sb.common_init(&joke_fstype, root);
	}

	return &joke_sb;
}

void 
joke_fstype_class::kill_sb(struct super_block * sb) 
{
	panic("joke sb deallocated");
}*/

#endif