#include <drivers/tty/tty.c>
#include <igris/util/bug.h>

int tty_write(struct node * node, const char * data, size_t size, int flags) 
{
	struct tty * tty = mcast_out(node, struct tty, node);
	BUG();
}

int tty_read(struct node * node, char * data, size_t size, int flags) 
{
	struct tty * tty = mcast_out(node, struct tty, node);
	BUG();
}

int tty_open(struct node * node, struct file * filp, int flags) 
{
	struct tty * tty = mcast_out(node, struct tty, node);

	tty -> opensch = current_schedee();
}

int tty_release(struct node * node, int flags) 
{
	struct tty * tty = mcast_out(node, struct tty, node);
	BUG();
}

struct tty_node_operations tty_node_ops = 
{
	.write = tty_write,	
	.read = tty_read,

	.open = tty_open,
	.release = tty_release,
};