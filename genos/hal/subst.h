//2012-2015 Mirmik 

#ifndef GENOS_SUBST_H
#define GENOS_SUBST_H
	
#include "sys/cdefs.h"
#include "arch/context.h"

struct context;

__BEGIN_DECLS

void context_switch (struct context* prev, struct context* next);
void context_init (struct context* ctx, uintptr_t stck, void(*func)(void*), void* param, uint8_t irqen);

//EXPERIMENTAL
void context_load(struct context* next);
void context_save_and_invoke_displace(struct context* cntxt);

void context_print (struct context* cntxt);

__END_DECLS

#endif
