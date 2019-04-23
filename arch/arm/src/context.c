#include <hal/subst.h>
#include <asm/context.h>
#include <string.h>

void context_init (struct context* ctx,
                   uintptr_t sp,
                   void(*func)(void*),
                   void* arg,
                   uint8_t irqen)
{
	memset(ctx, 0, sizeof(struct context));

	ctx->lr = 	(uint32_t) func;
	ctx->r[0] = (uint32_t) arg;
	ctx->sp = 	(uint32_t) sp;

//	if (flags & CONTEXT_PRIVELEGED) {
	ctx->control = 0;
//	} else {
//		ctx->control = CM3_CONTROL_NPRIV;
//	}
}
