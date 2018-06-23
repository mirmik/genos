//#include "genos_configure.h"
#include <asm/startup.h>
#include <avr/io.h>
#include <avr/common.h>
#include <avr/sfr_defs.h>

.global __start

/****************************************************************************
 * Reset Entry Point
 ****************************************************************************/

	.section .init, "ax", @progbits
	.func	__start
__start:

	/* disable interrupts. Useful for software reset by jumping to address 0 */
	cli
	clr r0
	clr r1
	clr r2	
	clr r3
	clr r4
	clr r5
	clr r6
	clr r7
	clr r8
	clr r9
	clr r10
	clr r11
	clr r12
	clr r13
	clr r14	
	clr r15
	clr r16
	clr r17
	clr r18
	clr r19
	clr r20
	clr r21
	clr r22
	clr r24
	clr r25
	clr r26
	clr r27
	clr r28
	clr r29
	clr r30
	clr r31

	/* set EIND to 0, just to be sure we are sane */
	ldi r16, 0
	out		_SFR_IO_ADDR(EIND), r16 // EIND = 0x3c
	ldi r16, 0
	out		_SFR_IO_ADDR(RAMPZ), r16

	/* Clear the zero register, clear the status register and initialize the
	 * IDLE thread stack
	 */

	clr		r1
	out		_SFR_IO_ADDR(SREG), r1
	ldi		r28, lo8(STACKBASE)
	ldi		r29, hi8(STACKBASE)
	out		_SFR_IO_ADDR(SPH), r29
	out		_SFR_IO_ADDR(SPL), r28
	
	/* Copy initial global data values from FLASH into RAM */

	.global	__do_copy_data;			/* Required to suppress dragging in logic from libgcc */
__do_copy_data:

#ifdef HAVE_RAMPZ
	ldi		r17, hi8(_edata)
	ldi		r26, lo8(_sdata)
	ldi		r27, hi8(_sdata)
	ldi		r30, lo8(_eronly)
	ldi		r31, hi8(_eronly)
	ldi		r16, hh8(_eronly)
	out		_SFR_IO_ADDR(RAMPZ), r16
	rjmp	.Lcopystart

.Lcopyloop:
	elpm	r0, Z+
	st		X+, r0

.Lcopystart:
	cpi		r26, lo8(_edata)
	cpc		r27, r17
	brne	.Lcopyloop
#else
	ldi		r17, hi8(_edata)
	ldi		r26, lo8(_sdata)
	ldi		r27, hi8(_sdata)
	ldi		r30, lo8(_eronly)
	ldi		r31, hi8(_eronly)
	rjmp	.Lcopystart

.Lcopyloop:
	lpm		r0, Z+
	st		X+, r0

.Lcopystart:
	cpi		r26, lo8(_edata)
	cpc		r27, r17
	brne	.Lcopyloop
#endif

	/* Clear uninitialized data */

	.global	__do_clear_bss;			/* Required to suppress dragging in logic from libgcc */
__do_clear_bss:

	ldi		r17, hi8(_ebss)
	ldi		r26, lo8(_sbss)
	ldi		r27, hi8(_sbss)
	ldi		r16, hh8(_ebss)
	out		_SFR_IO_ADDR(RAMPZ), r16
	rjmp	.Lclearstart

.Lclearloop:
	st		X+, r1

.Lclearstart:
	cpi		r26, lo8(_ebss)
	cpc		r27, r17
	brne	.Lclearloop

	/* Call global ctors */

.global __do_global_constructors
__do_global_constructors:
	ldi		r30, lo8(_ctors_start)
	ldi		r31, hi8(_ctors_start)
	ldi		r18, hi8(_ctors_end)
	ldi		r16, hh8(_ctors_end)
	out		_SFR_IO_ADDR(RAMPZ), r16
	
	rjmp	.Lctorsstart

.Lctorsloop:
	elpm	r16, Z+
	elpm	r17, Z+
	push r30
	push r31
	mov r30, r16
	mov r31, r17
	
	push r16
	push r17
	push r18

	icall
	
	pop r18
	pop	r17
	pop r16

	pop r31
	pop r30
	
.Lctorsstart:
	cpi		r30, lo8(_ctors_end)
	cpc		r31, r18
	brne	.Lctorsloop

	/* Start application */

	call	main

	/* If programm is complete, stub system */		
	
	jmp		end_of_programm

	.endfunc
