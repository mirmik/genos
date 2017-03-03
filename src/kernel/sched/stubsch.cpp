#include <kernel/sched/schedee.h>
#include <kernel/sched/stubschedee.h>

stub_schedee::stub_schedee() { 
	hops = &stub_schedee_service_operations;
}