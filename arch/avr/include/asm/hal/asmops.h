#ifndef SUBST_MACRO
#define SUBST_MACRO

#include "avr/io.h"

#define PROC_STATE_SIZE 33

//прямая установка StackPointer.
#define SP_SET(_SP) {SP=_SP;}   

//макросы для работы с SPstore. Запись и Чтение.
#define SP_GET() SP  

#define PUSH_STATE_REGS_TO_STACK() \
asm volatile ( "\n\t" \
"push r0 \n\t" \
"in r0, __SREG__ \n\t" \
"push r0 \n\t" \
"push r1 \n\t" \
"push r2 \n\t"  \
"push r3 \n\t" \
"push r4 \n\t" \
"push r5 \n\t" \
"push r6 \n\t" \
"push r7 \n\t" \
"push r8 \n\t" \
"push r9 \n\t" \
"push r10 \n\t" \
"push r11 \n\t" \
"push r12 \n\t" \
"push r13 \n\t" \
"push r14 \n\t" \
"push r15 \n\t" \
"push r16 \n\t" \
"push r17 \n\t" \
"push r18 \n\t" \
"push r19 \n\t"\
"push r20 \n\t" \
"push r21 \n\t" \
"push r22 \n\t" \
"push r23 \n\t"\
"push r24 \n\t" \
"push r25 \n\t" \
"push r26 \n\t" \
"push r27 \n\t" \
"push r28 \n\t" \
"push r29 \n\t"\
"push r30 \n\t" \
"push r31 \n\t");

#define POP_STATE_REGS_FROM_STACK() \
asm volatile ("\n\t" \
"pop r31 \n\t" \
"pop r30 \n\t" \
"pop r29 \n\t" \
"pop r28 \n\t" \
"pop r27 \n\t" \
"pop r26 \n\t" \
"pop r25 \n\t" \
"pop r24 \n\t"\
"pop r23 \n\t" \
"pop r22 \n\t" \
"pop r21 \n\t" \
"pop r20 \n\t" \
"pop r19 \n\t" \
"pop r18 \n\t" \
"pop r17 \n\t" \
"pop r16 \n\t" \
"pop r15 \n\t" \
"pop r14 \n\t"\
"pop r13 \n\t" \
"pop r12 \n\t" \
"pop r11 \n\t" \
"pop r10 \n\t" \
"pop r9 \n\t" \
"pop r8 \n\t" \
"pop r7 \n\t" \
"pop r6 \n\t" \
"pop r5 \n\t" \
"pop r4 \n\t"\
"pop r3 \n\t" \
"pop r2 \n\t" \
"pop r1 \n\t" \
"pop r0 \n\t" \
"out __SREG__, r0 \n\t" \
"pop r0 \n\t");

#define static_long_goto(a) asm("rjmp " #a);
#define long_goto(a) asm("ijmp" ::"z"(a));
#define drop_stack() setSP(::current_thread()->stack_top)

#endif
