#ifndef ZILLOT_CONTEXT_H
#define ZILLOT_CONTEXT_H

#include <igris/compiler.h>
#include <stdint.h>

#if !__NOARCH
#include <asm/context.h>
#endif

struct context;

__BEGIN_DECLS

void context_switch(struct context *save, struct context *load);
void context_init(struct context *ctx,
                  uintptr_t stck,
                  void (*func)(void *),
                  void *param,
                  uint8_t irqen);

int context_save(struct context *next, int code);
void context_load(struct context *next);
void context_save_and_displace(struct context *cntxt);

void context_dprint(struct context *cntxt);
void __context_drop__();
void context_set_variable(struct context *ctx, int val);

void *context_get_sp(struct context *ctx);
void context_set_sp(struct context *ctx, void *sp);

__END_DECLS

#endif
