.globl		__start
#define IDLE_STACK _estack

.macro	HANDLER, label, irqno
.thumb_func
	\label:
		mov		r0, #\irqno
		b		exception_common
.endm


.section	.vectors, "ax"
.code		16
.align		2
.global		_vectors

_vectors:

/* Processor Exceptions */
	.word	IDLE_STACK			/* Vector  0: Reset stack pointer */
	.word	__start				/* Vector  1: Reset vector */
	.word	vect_nmi			/* Vector  2: Non-Maskable Interrupt (NMI) */
	.word	vect_hardfault		/* Vector  3: Hard fault */
	.word	vect_mpu			/* Vector  4: Memory management (MPU) */
	.word	vect_busfault		/* Vector  5: Bus fault */
	.word	vect_usagefault		/* Vector  6: Usage fault */
	.word	vect_reserved		/* Vector  7: Reserved */
	.word	vect_reserved		/* Vector  8: Reserved */
	.word	vect_reserved		/* Vector  9: Reserved */
	.word	vect_reserved		/* Vector 10: Reserved */
	.word	vect_svcall			/* Vector 11: SVC call */
	.word	vect_dbgmonitor		/* Vector 12: Debug monitor */
	.word	vect_reserved		/* Vector 13: Reserved */
	.word	vect_pendsv			/* Vector 14: Pendable system service request */
	.word	vect_systick		/* Vector 15: System tick */

/* TODO External exception */

	.size	_vectors, .-_vectors

/************************************************************************************
 * .text
 ************************************************************************************/

	.text
	.type	handlers, function
	.thumb_func
handlers:
	HANDLER	vect_reserved, 		0/* Unexpected/reserved vector */
	HANDLER	vect_nmi, 			1/* Vector  2: Non-Maskable Interrupt (NMI) */
	HANDLER	vect_hardfault, 	2/* Vector  3: Hard fault */
	HANDLER	vect_mpu, 			3/* Vector  4: Memory management (MPU) */
	HANDLER	vect_busfault, 		4/* Vector  5: Bus fault */
	HANDLER	vect_usagefault, 	5/* Vector  6: Usage fault */
	HANDLER	vect_svcall, 		6/* Vector 11: SVC call */
	HANDLER	vect_dbgmonitor, 	7/* Vector 12: Debug Monitor */
	HANDLER	vect_pendsv, 		8/* Vector 14: Penable system service request */
	HANDLER	vect_systick, 		9/* Vector 15: System tick */

/* Common IRQ handling logic.  On entry here, the return stack is on either
 * the PSP or the MSP and looks like the following:
 *
 *      REG_XPSR
 *      REG_R15
 *      REG_R14
 *      REG_R12
 *      REG_R3
 *      REG_R2
 *      REG_R1
 * MSP->REG_R0
 *
 * And
 *      R0 contains the IRQ number
 *      R14 Contains the EXC_RETURN value
 *      We are in handler mode and the current SP is the MSP
 */

.extern do_irq
.globl		exception_common
.type		exception_common, function

exception_common:
	push {lr}
	#bl do_irq
	pop {pc}
