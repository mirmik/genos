#include <asm/context.h>
#include <string.h>
#include <igris/dprint/dprint.h>
#include <igris/util/access.h>

void context_init (struct context* ctx, uintptr_t stck, void(*func)(void*), void* param, uint8_t irqen) {
	memset(ctx, 0, sizeof(struct context));
	ctx->regs[1] = 0;
	ctx->control = irqen ? 0x80 : 0;
	ctx->sp = stck;
	ctx->pc = (uintptr_t)func;
	uintptr_t aparam = (uintptr_t) param;
	ctx->regs[24] = UINT16_LO(aparam);
	ctx->regs[25] = UINT16_HI(aparam);
}