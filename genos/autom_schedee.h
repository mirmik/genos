#ifndef GENOS_AUTOM_SCHEDEE_H
#define GENOS_AUTOM_SCHEDEE_H

#include <genos/schedee.h> 
#include <igris/compiler.h>

typedef void (* autom_schedee_func_t)(void* priv, int* state);

struct autom_schedee 
{
	struct schedee sch;
	int state;

	autom_schedee_func_t func;
	void *               priv;
};

__BEGIN_DECLS

void autom_schedee_init(struct autom_schedee * sch, autom_schedee_func_t foo, void * privdata);

__END_DECLS

#endif