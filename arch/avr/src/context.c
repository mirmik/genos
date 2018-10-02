#include <hal/subst.h>
#include <string.h>
#include <gxx/debug/dprint.h>
#include <util/access.h>

void context_print(struct context* ctx)
{
	debug_print("fixed_r0_tmp:\t");debug_printhex_uint8(ctx->regs[0]);dln();
	debug_print("fixed_r1_null:\t");debug_printhex_uint8(ctx->regs[1]);dln();
	debug_print("saved_r2:\t");debug_printhex_uint8(ctx->regs[2]);dln();
	debug_print("saved_r3:\t");debug_printhex_uint8(ctx->regs[3]);dln();
	debug_print("saved_r4:\t");debug_printhex_uint8(ctx->regs[4]);dln();
	debug_print("saved_r5:\t");debug_printhex_uint8(ctx->regs[5]);dln();
	debug_print("saved_r6:\t");debug_printhex_uint8(ctx->regs[6]);dln();
	debug_print("saved_r7:\t");debug_printhex_uint8(ctx->regs[7]);dln();
	debug_print("saved_r8:\t");debug_printhex_uint8(ctx->regs[8]);dln();
	debug_print("saved_r9:\t");debug_printhex_uint8(ctx->regs[9]);dln();
	debug_print("saved_r10:\t");debug_printhex_uint8(ctx->regs[10]);dln();
	debug_print("saved_r11:\t");debug_printhex_uint8(ctx->regs[11]);dln();
	debug_print("saved_r12:\t");debug_printhex_uint8(ctx->regs[12]);dln();
	debug_print("saved_r13:\t");debug_printhex_uint8(ctx->regs[13]);dln();
	debug_print("saved_r14:\t");debug_printhex_uint8(ctx->regs[14]);dln();
	debug_print("saved_r15:\t");debug_printhex_uint8(ctx->regs[15]);dln();
	debug_print("saved_r16:\t");debug_printhex_uint8(ctx->regs[16]);dln();
	debug_print("saved_r17:\t");debug_printhex_uint8(ctx->regs[17]);dln();
	debug_print("used_r18:\t");debug_printhex_uint8(ctx->regs[18]);dln();
	debug_print("used_r19:\t");debug_printhex_uint8(ctx->regs[19]);dln();
	debug_print("used_r20:\t");debug_printhex_uint8(ctx->regs[20]);dln();
	debug_print("used_r21:\t");debug_printhex_uint8(ctx->regs[21]);dln();
	debug_print("used_r22:\t");debug_printhex_uint8(ctx->regs[22]);dln();
	debug_print("used_r23:\t");debug_printhex_uint8(ctx->regs[23]);dln();
	debug_print("used_r24:\t");debug_printhex_uint8(ctx->regs[24]);dln();
	debug_print("used_r25:\t");debug_printhex_uint8(ctx->regs[25]);dln();
	debug_print("used_r26_xlo:\t");debug_printhex_uint8(ctx->regs[26]);dln();
	debug_print("used_r27_xhi:\t");debug_printhex_uint8(ctx->regs[27]);dln();
	debug_print("saved_r28_y_lo:\t");debug_printhex_uint8(ctx->regs[28]);dln();
	debug_print("saved_r29_y_hi:\t");debug_printhex_uint8(ctx->regs[29]);dln();
	debug_print("used_r30_z_lo:\t");debug_printhex_uint8(ctx->regs[30]);dln();
	debug_print("used_r31_z_hi:\t");debug_printhex_uint8(ctx->regs[31]);dln();	
	debug_print("pc:\t\t");debug_printhex_uint16(ctx->pc);dln();
	debug_print("sp:\t\t");debug_printhex_uint16(ctx->sp);dln();
	debug_print("sreg:\t\t");debug_printhex_uint8(ctx->control);dln();
}

void context_init (struct context* ctx, uintptr_t stck, void(*func)(void*), void* param, uint8_t irqen) {
	memset(ctx, 0, sizeof(struct context));
	ctx->regs[1] = 0;
	ctx->control = irqen ? 0x80 : 0;
	ctx->sp = stck;
	ctx->pc = (uintptr_t)func;
	//*(void**)&ctx->regs[24] = param;
	uintptr_t aparam = (uintptr_t) param;
	ctx->regs[24] = UINT16_LO(aparam);
	ctx->regs[25] = UINT16_HI(aparam);
}