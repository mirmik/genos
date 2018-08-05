#include <avr/io.h>

.global context_switch
context_switch:

push r28
push r29

mov r29, r25
mov r28, r24
	
st y+, r0 		; regs
st y+, r1
st y+, r2
st y+, r3
st y+, r4
st y+, r5
st y+, r6
st y+, r7
st y+, r8
st y+, r9
st y+, r10
st y+, r11
st y+, r12
st y+, r13
st y+, r14
st y+, r15
st y+, r16
st y+, r17
st y+, r18
st y+, r19
st y+, r20
st y+, r21
st y+, r22
st y+, r23
st y+, r24
st y+, r25
st y+, r26
st y+, r27

pop r27
pop r26
st y+, r26
st y+, r27

st y+, r30
st y+, r31

pop r0 		; pc counter
pop r16
pop r0
st y+, r0
st y+, r16

in r0, AVR_STACK_POINTER_LO_ADDR	; stack pointer
st y+, r0
in r0, AVR_STACK_POINTER_HI_ADDR
st y+, r0

in r0, AVR_STATUS_ADDR
st y+, r0

mov r29, r23
mov r28, r22

ldd r0, Y+32+4
out AVR_STATUS_ADDR, r0

ldd r0, Y+32+2
out AVR_STACK_POINTER_LO_ADDR, r0	; stack pointer
ldd r0, Y+32+3
out AVR_STACK_POINTER_HI_ADDR, r0

ldd r0, Y+32+0 
push r0
ldd r0, Y+32+1 
push r0
ldi r16, 0x00
push r16

ld r0, y+ 		; regs
ld r1, y+
ld r2, y+
ld r3, y+
ld r4, y+
ld r5, y+
ld r6, y+
ld r7, y+
ld r8, y+
ld r9, y+
ld r10, y+
ld r11, y+
ld r12, y+
ld r13, y+
ld r14, y+
ld r15, y+
ld r16, y+
ld r17, y+
ld r18, y+
ld r19, y+
ld r20, y+
ld r21, y+
ld r22, y+
ld r23, y+
ld r24, y+
ld r25, y+
ld r26, y+
ld r27, y+

push r26
push r27
ld r26, y+
ld r27, y+

ld r30, y+
ld r31, y+

mov r28, r26
mov r29, r27
pop r27
pop r26

ret






.global context_load
context_load:

mov r29, r25
mov r28, r24

ldd r0, Y+32+4
out AVR_STATUS_ADDR, r0

ldd r0, Y+32+2
out AVR_STACK_POINTER_LO_ADDR, r0	; stack pointer
ldd r0, Y+32+3
out AVR_STACK_POINTER_HI_ADDR, r0

ldd r0, Y+32+0 
push r0
ldd r0, Y+32+1 
push r0

//TODO: RAMPZ???
//ldi r16, 0x00
//push r16

ld r0, y+ 		; regs
ld r1, y+
ld r2, y+
ld r3, y+
ld r4, y+
ld r5, y+
ld r6, y+
ld r7, y+
ld r8, y+
ld r9, y+
ld r10, y+
ld r11, y+
ld r12, y+
ld r13, y+
ld r14, y+
ld r15, y+
ld r16, y+
ld r17, y+
ld r18, y+
ld r19, y+
ld r20, y+
ld r21, y+
ld r22, y+
ld r23, y+
ld r24, y+
ld r25, y+
ld r26, y+
ld r27, y+

push r26
push r27
ld r26, y+
ld r27, y+

ld r30, y+
ld r31, y+

mov r28, r26
mov r29, r27
pop r27
pop r26

ret





.global __displace__
.global context_save_and_invoke_displace
context_save_and_invoke_displace:

push r28
push r29

mov r29, r25
mov r28, r24
	
st y+, r0 		; regs
st y+, r1
st y+, r2
st y+, r3
st y+, r4
st y+, r5
st y+, r6
st y+, r7
st y+, r8
st y+, r9
st y+, r10
st y+, r11
st y+, r12
st y+, r13
st y+, r14
st y+, r15
st y+, r16
st y+, r17
st y+, r18
st y+, r19
st y+, r20
st y+, r21
st y+, r22
st y+, r23
st y+, r24
st y+, r25
st y+, r26
st y+, r27

pop r27
pop r26
st y+, r26
st y+, r27

st y+, r30
st y+, r31

//TODO: RAMPZ?
//pop r0 		; pc counter
pop r16
pop r0

st y+, r0
st y+, r16

in r0, AVR_STACK_POINTER_LO_ADDR	; stack pointer
st y+, r0
in r0, AVR_STACK_POINTER_HI_ADDR
st y+, r0

in r0, AVR_STATUS_ADDR
st y+, r0

jmp __displace__


.global genos_context_displace
genos_context_displace:

call context_save_and_invoke_displace
ret