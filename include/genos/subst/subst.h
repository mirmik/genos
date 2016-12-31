//2012-2015 Mirmik 

#ifndef GENOS_SUBST_H
#define GENOS_SUBST_H
	
#include "compiler.h"
#include "asm/context.h"

struct context;

__BEGIN_DECLS

void context_switch (struct context* prev, struct context* next);
void context_init (struct context* ctx, uintptr_t stck, void(*func)(void*), void* param);

__END_DECLS

#endif
