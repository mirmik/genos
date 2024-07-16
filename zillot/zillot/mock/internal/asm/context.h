#ifndef ZILLOT_MOCK_ASM_CONTEXT_H
#define ZILLOT_MOCK_ASM_CONTEXT_H

#include <igris/compiler.h>
#include <ucontext.h>

struct context
{
    int variable;
    struct ucontext_t uc;
};

__BEGIN_DECLS
void set_anchor_context(struct ucontext_t *uc);
__END_DECLS

#endif