#include <mvfs/mvfs.h>
#include <sched/sched.h>

struct node * vfs_get_pwd() {
	struct schedee * sch = current_schedee();
	if (sch == NULL) return vfs_get_root();
	return sch->pwd;
}

void vfs_set_pwd(struct node * d) {
	current_schedee()->pwd = d; 
}