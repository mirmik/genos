#include <gxx/debug/dprint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sched/sched.h>
#include <sched/api.h>

void* task0_1(void* arg, int* state) {
	(void) arg;
	
	switch (*state) 
	{
		case 0:
			dprln("task0_1_0");
			*state = 1;
			break;
		case 1:
			dprln("task0_1_1");			
			schedee_exit();
			break;
	}
}

void* task0(void* arg, int* state) {
	(void) arg;
	int sts;

	switch (*state) 
	{
		case 0:
			dprln("task0_0");
			schedee_run(create_autom_schedee(task0_1, NULL));
			*state = 1;
			break;
		case 1:
			dprln("task0_1");
			schedee_exit();
			break;
	}
}

void* task1(void* arg, int* state) {
	(void) arg;
	int sts;

	switch (*state) 
	{
		case 0:
			dprln("task1_0");
			*state = 1;
			break;
		case 1:
			dprln("task1_1");
			schedee_exit();
			break;
	}
}

int main() {
	schedee_manager_init();

	schedee_run(create_autom_schedee(task0, NULL));
	schedee_run(create_autom_schedee(task1, NULL));

	__schedule__();
}

void __schedule__() {
	while(1) {
		if (genos::schedee_manager::total_planed() == 0) exit(0);
		genos::schedee_manager::step();
	}
}