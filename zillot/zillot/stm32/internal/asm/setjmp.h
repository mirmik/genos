#ifndef STM32_ASM_SETJMP_H_
#define STM32_ASM_SETJMP_H_

#include <asm/context.h>
#include <zillot/context.h>

struct __jmp_buf_s
{
    struct context ctx;
    volatile int val;
};

typedef struct __jmp_buf_s __jmp_buf[1];

__BEGIN_DECLS
int setjmp(struct __jmp_buf_s *buf);
void longjmp(struct __jmp_buf_s *buf, int val);
__END_DECLS

#endif