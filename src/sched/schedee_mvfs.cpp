#include "schedee.h"
#include <mvfs/compat.h>
#include <mvfs/file.h>

void schedee_mvfs_support_init(struct schedee* sch) {
	sch->pwd = sch->parent ? sch->parent->pwd : vfs_get_root();
	for (int i = 0; i < SCHEDEE_FDMAX; ++i) {
		//	dpr("init "); dprln(i);
		sch->fds[i] = NULL;
	}
}

void schedee_copy_parent_files(struct schedee* sch) {
	for (int i = 0; i < SCHEDEE_FDMAX; ++i) {
		sch->fds[i] = sch->parent->fds[i];
		if (sch->fds[i] != NULL) {
			//dpr("copy "); dprhexln(sch->fds[i]);
			sch->fds[i]->refs++;
		}
	}
}

void schedee_debug_print_fds(struct schedee* sch) {
	dpr("schedee "); 
	dprptr(sch); 
	dpr(": ");
	for (int i = 0; i < SCHEDEE_FDMAX; ++i) {
		dprptr(sch->fds[i]); 
		dpr(" ");
	}
	dln();
}