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

// @r0 - context address
// @return - save stored in r1 register
.global context_load
context_load:
    // Copy context address to r1
    mov		r1, r0

    // Load SP from context
	ldr     sp, [r1, #56]

    // Load CONTROL register from context
    ldr     r2, [r1, #60]
    msr     CONTROL, r2

    // Load registers from context
    add     r0, r1, #56
    ldmdb   r0, {r2 - r12, r14}

    // Load r0 and r1 from context
    ldr     r0, [r1]
    ldr     r1, [r1, #4]

    // Return to caller
    mov	    pc, lr
	
.global context_save_and_displace
context_save_and_displace:
    // Store registers in context
    stmia   r0, {r0 - r12, r14}

    // Store SP in context
    str     sp, [r0, #56]

    // Store CONTROL register in context
    mrs     r2, CONTROL
    str     r2, [r0, #60]

    // Go to displace function
    ldr     pc, =__context_displace_vector__


// context_save записывает текущее состояние процесса.
// context_load востановит контекст, как если бы 
// функция вернулась context_save
// Функция получает на вход аргумент r1, но он может быть перезаписан
// для различения контекста при копировании
// @ cntxt
// @ val - аргумент для возврата
// @ return - возвращает значение val
.global context_save
context_save:
    // Move data to store second argument in r0 register
    mov r3, r0
    mov r0, r1

    // Store registers in context
    stmia   r3, {r0 - r12, r14}

    // Store SP in context
    str     sp, [r3, #56]

     // Store CONTROL register in context
    mrs     r2, CONTROL
    str     r2, [r3, #60]

    // Return to caller
    mov	    pc, lr

.global setjmp
setjmp:
    stmia   r0, {r0 - r12, r14}
    str     sp, [r0, #56]
    mrs     r2, CONTROL
    str     r2, [r0, #60]
    
    ldr     r0, =0
    mov	    pc, lr

.global longjmp
longjmp:
    str     r1, [r0, #64]

	mov		r1, r0
	ldr     sp, [r1, #56]
    ldr     r2, [r1, #60]
    msr     CONTROL, r2

    add     r0, r1, #56
    ldmdb   r0, {r2 - r12, r14}
    ldr     r0, [r1]
    ldr     r1, [r1, #4]

    mov	    pc, lr