#include <kernel/sched/schedee.h>

stub_schedee::stub_schedee() { 
	//cops = NULL;
	hops = NULL;
	cops = &stub_schedee_service_operations;
}