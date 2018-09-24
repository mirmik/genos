#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sched/sched.h>
#include <sched/api.h>

#include <gxx/debug/dprint.h>

#include <mvfs/mvfs.h>
#include <mvfs/fsops.h>
#include <mvfs/fs/joke/joke.h>

int task0_1(void* arg, int* state) {
	(void) arg;
	
	switch (*state) 
	{
		case 0:
			dprln("task0_1_0");

			*state = 1;
			__displace__();
			break;
		case 1:
			dprln("task0_1_1");
			vfs_debug_pwd();
			
			schedee_exit();
			break;
	}
}

int task0(void* arg, int* state) {
	(void) arg;

	switch (*state) 
	{
		case 0:
			dprln("task0_0");

			int sts = vfs_chdir("/home/mirmik/project");
				if (sts) dprf("%s\n", strerror(sts));

			schedee_run(create_autom_schedee(task0_1, NULL));

			*state = 1;
			__displace__();
			break;
		case 1:
			dprln("task0_1");
			vfs_debug_pwd();
			
			schedee_exit();
			break;
	}
}

int task1(void* arg, int* state) {
	(void) arg;

	switch (*state) 
	{
		case 0:
			dprln("task1_0");

			int sts = vfs_chdir("/home/mirmik");
				if (sts) dprf("%s\n", strerror(sts));

			*state = 1;
			__displace__();
			break;
		case 1:
			dprln("task1_1");
			vfs_debug_pwd();
			
			schedee_exit();
			break;
	}
}

int main() {
	schedee_manager_init();

	vfs_register_fs(&joke_fstype);

	int sts = vfs_mount_first("joke", 0, NULL);
		if (sts) dprf("%s\n", strerror(sts));

	sts = vfs_mkdir("/home");
		if (sts) dprf("%s\n", strerror(sts));
	sts = vfs_mkdir("/home/mirmik");
		if (sts) dprf("%s\n", strerror(sts));
	sts = vfs_mkdir("/home/mirmik/project");
		if (sts) dprf("%s\n", strerror(sts));
	sts = vfs_mkdir("/home/mirmik/test");
		if (sts) dprf("%s\n", strerror(sts));

	schedee_run(create_autom_schedee(task0, NULL));
	schedee_run(create_autom_schedee(task1, NULL));

	__schedule__();
}

void __schedule__() {
	while(1) {
		schedee_manager();
	}
}