#include <asm/context.h>
#include <stddef.h>
#include <zillot/context.h>

struct ucontext_t *_anchor_context = NULL;

void context_switch(struct context *save, struct context *load)
{
    swapcontext(&save->uc, &load->uc);
}

void context_init(struct context *ctx,
                  uintptr_t stck,
                  void (*func)(void *),
                  void *param,
                  uint8_t irqen)
{
    makecontext(&ctx->uc, (void (*)())func, 1, param);
}

void context_load(struct context *next)
{
    setcontext(&next->uc);
}

int context_save(struct context *cntxt, int val)
{
    cntxt->variable = val;
    return getcontext(&cntxt->uc);
}

void context_save_and_displace(struct context *cntxt)
{
    getcontext(&cntxt->uc);
}

void set_anchor_context(struct ucontext_t *uc)
{
    _anchor_context = uc;
}

void context_dprint(struct context *cntxt) {}

void __context_drop__()
{
    setcontext(_anchor_context);
}

void context_set_variable(struct context *ctx, int val)
{
    ctx->variable = val;
}

void *context_get_sp(struct context *ctx)
{
    return ctx->uc.uc_stack.ss_sp;
}

void context_set_sp(struct context *ctx, void *sp)
{
    ctx->uc.uc_stack.ss_sp = sp;
}