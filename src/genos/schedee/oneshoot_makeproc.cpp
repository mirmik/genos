#include <igris/shell/executor.h>
#include <genos/resource.h>

#include <genos/schedee/coop.h>

struct utildata_struct {
	char stack[512];
	int(*func)(int, char**);
	int argc;
	char argv_s[16][8];
	char * argv[8];
} utildata;

void * starter(void * priv) 
{
	utildata.func(utildata.argc, utildata.argv);
	return nullptr;
} 

genos::coopschedee utilschedee;

void executor_make_process(
	int(*func)(int,char**), 
	int argc, char** argv) 
{
	utildata.argc = argc;
	utildata.func = func;

	for (int i = 0; i < argc; ++i) 
	{
		strcpy(utildata.argv_s[i], argv[i]);
		utildata.argv[i] = utildata.argv_s[i];
	}

	utilschedee.init(
		starter, 
		(void*)&utildata, 
		utildata.stack, 
		512,
		SCHEDEE_USE_PARENT_GID);

	genos::schedee * parent = current_schedee();
	utilschedee.attach_restbl(parent->restbl);

	utilschedee.start();
}
