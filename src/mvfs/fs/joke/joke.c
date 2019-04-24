#include <mvfs/fs/joke/joke.h>
#include <igris/util/bug.h>

struct joke_node * joke_node_create(const char * name, size_t nlen) {
	struct joke_node * ret = (struct joke_node *) malloc(sizeof(struct joke_node));;
	node_init(&ret->node, name, nlen);
	return ret;
}

struct joke_node * joke_node_create_as_child(const char * name, size_t nlen,
	struct node * parent
) {
	struct joke_node * ret = joke_node_create(name, nlen);
	node_add_child(&ret->node, parent);
	return ret;
}

struct super_block * joke_get_sb(struct file_system_type * fs, int flags, const void * data) 
{
	return &joke_sb.sb;
}

void joke_kill_sb(struct file_system_type * fs, struct super_block * sb) 
{
	BUG();
}

const struct file_system_operations joke_fs_ops = 
{
	.get_sb = joke_get_sb,
	.kill_sb = joke_kill_sb
};

const struct node_operations joke_node_ops = 
{

};

struct joke_node joke_root = { NODE_INIT(joke_root.node, "/", &joke_sb.sb, &joke_node_ops, NODE_DIRECTORY) };
struct joke_fstype_class joke_fstype = { FILE_SYSTEM_INIT(joke_fstype.fs, "joke", &joke_fs_ops) };
struct joke_super_block joke_sb = { SUPER_BLOCK_INIT(joke_sb.sb, &joke_fstype.fs, &joke_root.node) };