#include <gxx/debug/assembler.h>

#include "avr/io.h"

.macro print_reg p1
push r0
mov r24, \p1
rcall debug_printhex_uint8
rcall debug_print_line
pop r0
.endm

dasm_regmap_set:
push r27
push r26
push r16
sts debug_regmap + 0, r0
sts debug_regmap + 1, r1
sts debug_regmap + 2, r2
sts debug_regmap + 3, r3
sts debug_regmap + 4, r4
sts debug_regmap + 5, r5
sts debug_regmap + 6, r6
sts debug_regmap + 7, r7
sts debug_regmap + 8, r8
sts debug_regmap + 9, r9
sts debug_regmap + 10, r10
sts debug_regmap + 11, r11
sts debug_regmap + 12, r12
sts debug_regmap + 13, r13
sts debug_regmap + 14, r14
sts debug_regmap + 15, r15
sts debug_regmap + 16, r16
sts debug_regmap + 17, r17
sts debug_regmap + 18, r18
sts debug_regmap + 19, r19
sts debug_regmap + 20, r20
sts debug_regmap + 21, r21
sts debug_regmap + 22, r22
sts debug_regmap + 23, r23
sts debug_regmap + 24, r24
sts debug_regmap + 25, r25
sts debug_regmap + 26, r26
sts debug_regmap + 27, r27
sts debug_regmap + 28, r28
sts debug_regmap + 29, r29
sts debug_regmap + 30, r30
sts debug_regmap + 31, r31
in r16, AVR_STATUS_ADDR
sts debug_regmap + 32, r16
in r26, AVR_STACK_POINTER_LO_ADDR
sts debug_regmap + 33, r26
in r27, AVR_STACK_POINTER_HI_ADDR
sts debug_regmap + 34, r27

adiw r26, 1+3+1
ld r16,X+
sts debug_regmap + 36, r16
ld r16,X+
sts debug_regmap + 35, r16

pop r16
pop r26
pop r27
ret

;.global dasm_stack_dump
;.global debug_print_dump
;dasm_stack_dump:
;mov r23,r25
;mov r22,r24
;in r24, AVR_STACK_POINTER_LO_ADDR
;in r25, AVR_STACK_POINTER_HI_ADDR
;rcall debug_print_dump
;ret
