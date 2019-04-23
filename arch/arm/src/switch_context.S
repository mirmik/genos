.global __context_drop__

.text
.thumb
.syntax unified

.global context_switch
context_switch:
    stmia   r0, {r0 - r12, r14}
    str	    sp, [r0, #56]
    mrs     r2, CONTROL
    str     r2, [r0, #60]

    ldr     sp, [r1, #56]
    ldr     r2, [r1, #60]
    msr     CONTROL, r2

    add     r0, r1, #56
    ldmdb   r0, {r2 - r12, r14}
    ldr     r0, [r1]
    ldr     r1, [r1, #4]

    mov	    pc, lr

.global context_load
context_load:
	mov		r1, r0
	ldr     sp, [r1, #56]
    ldr     r2, [r1, #60]
    msr     CONTROL, r2

    add     r0, r1, #56
    ldmdb   r0, {r2 - r12, r14}
    ldr     r0, [r1]
    ldr     r1, [r1, #4]

    mov	    pc, lr
	
.global context_save_and_displace
context_save_and_displace:
    stmia   r0, {r0 - r12, r14}
    str     sp, [r0, #56]
    mrs     r2, CONTROL
    str     r2, [r0, #60]

    ldr     pc, =__context_displace_vector__
