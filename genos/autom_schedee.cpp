#include <genos/autom_schedee.h>
#include <genos/displace.h>
#include <igris/util/bug.h>

void genos::autom_schedee::execute() 
{
	func(priv, &state);
}

void genos::autom_schedee::finalize() 
{}

int genos::autom_schedee::displace() 
{
	BUG();
	return 0;
}

genos::autom_schedee::autom_schedee(autom_schedee_func_t foo, void * privdata) 
	: schedee(SCHEDEE_PRIORITY_TOTAL - 1, 0)
{
	this->state = 0;
	this->func = foo;
	this->priv = privdata;
}