#include <igris/dprint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <genos/sched.h>
#include <genos/schedee/autom.h>
#include <genos/api.h>

extern genos::autom_schedee sch0_1;

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
			sch0_1.start();
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

genos::autom_schedee sch0 {task0, NULL};
genos::autom_schedee sch1 {task1, NULL};
genos::autom_schedee sch0_1 {task0_1, NULL};

int main() 
{
	dprln("start");
	schedee_manager_init();

	sch0.start();
	sch1.start();

	__schedule__();
}

void __schedule__() {
	while(1) {
		dpr("total_planned: ", schedee_manager_total_planed()); dprln();
		if (schedee_manager_total_planed() == 0) 
		{
			dprln("all good");
			exit(0);
		}
		schedee_manager_step();
	}
}