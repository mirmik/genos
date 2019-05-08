#include <periph/irqdefs.h>
#include <asm/chip.h>

#define IDLE_STACK _estack

.globl		__start
.global 	irq_stub
.global 	light

.macro	HANDLER, label, irqno
.thumb_func
	\label:
		mov		r0, #\irqno
		b		exception_common
.endm

/************************************************************************************
 * Vectors
 ************************************************************************************/

.section	.vectors, "ax"
.code		16
.align		2
.global		_vectors

_vectors:

/* Processor Exceptions */

	.word	IDLE_STACK			/* Vector  0: Reset stack pointer */
	.word	__start				/* Vector  1: Reset vector */
	.word	stm32_nmi			/* Vector  2: Non-Maskable Interrupt (NMI) */
	.word	hardfault_handler	/* Vector  3: Hard fault */
	.word	stm32_mpu			/* Vector  4: Memory management (MPU) */
	.word	stm32_busfault		/* Vector  5: Bus fault */
	.word	stm32_usagefault	/* Vector  6: Usage fault */
	.word	stm32_reserved		/* Vector  7: Reserved */
	.word	stm32_reserved		/* Vector  8: Reserved */
	.word	stm32_reserved		/* Vector  9: Reserved */
	.word	stm32_reserved		/* Vector 10: Reserved */
	.word	stm32_svcall		/* Vector 11: SVC call */
	.word	stm32_dbgmonitor	/* Vector 12: Debug monitor */
	.word	stm32_reserved		/* Vector 13: Reserved */
	.word	stm32_pendsv		/* Vector 14: Pendable system service request */
	.word	stm32_systick		/* Vector 15: System tick */

/* External Interrupts */

#if !defined(CHIP_NOEXT_VECTORS)
#undef VECTOR
#define VECTOR(l,i) .word l

#undef UNUSED
#define UNUSED(i) .word stm32_reserved

#if defined(CHIP_STM32L15XX)
#  include "chip//stm32l15xxx_vectors.h"
#elif defined(CHIP_STM32F10XX)
#  include "chip//stm32f10xxx_vectors.h"
#elif defined(CHIP_STM32F20XX)
#  include "chip//stm32f20xxx_vectors.h"
#elif defined(CHIP_STM32F30XX)
#  include "chip//stm32f30xxx_vectors.h"
#elif defined(CHIP_STM32F37XX)
#  include "chip//stm32f37xxx_vectors.h"
#elif defined(CHIP_STM32F40XX)
#  include "chip//stm32f40xxx_vectors.h"
#else
#  error "No vectors for STM32 chip"
#endif
#endif /* CHIP_NOEXT_VECTORS */
	.size	_vectors, .-_vectors

/************************************************************************************
 * .text
 ************************************************************************************/

	.text
	.type	handlers, function
	.thumb_func
handlers:
	HANDLER	stm32_reserved, STM32_IRQ_RESERVED		/* Unexpected/reserved vector */
	HANDLER	stm32_nmi, STM32_IRQ_NMI				/* Vector  2: Non-Maskable Interrupt (NMI) */
	HANDLER	stm32_hardfault, STM32_IRQ_HARDFAULT	/* Vector  3: Hard fault */
	HANDLER	stm32_mpu, STM32_IRQ_MEMFAULT			/* Vector  4: Memory management (MPU) */
	HANDLER	stm32_busfault, STM32_IRQ_BUSFAULT		/* Vector  5: Bus fault */
	HANDLER	stm32_usagefault, STM32_IRQ_USAGEFAULT	/* Vector  6: Usage fault */
	HANDLER	stm32_svcall, STM32_IRQ_SVCALL			/* Vector 11: SVC call */
	HANDLER	stm32_dbgmonitor, STM32_IRQ_DBGMONITOR	/* Vector 12: Debug Monitor */
	HANDLER	stm32_pendsv, STM32_IRQ_PENDSV			/* Vector 14: Penable system service request */
	HANDLER	stm32_systick, STM32_IRQ_SYSTICK		/* Vector 15: System tick */

#if !defined(CHIP_NOEXT_VECTORS)

#undef VECTOR
#define VECTOR(l,i) HANDLER l, i

#undef UNUSED
#define UNUSED(i)

#if defined(CHIP_STM32L15XX)
#  include "chip//stm32l15xxx_vectors.h"
#elif defined(CHIP_STM32F10XX)
#  include "chip//stm32f10xxx_vectors.h"
#elif defined(CHIP_STM32F20XX)
#  include "chip//stm32f20xxx_vectors.h"
#elif defined(CHIP_STM32F30XX)
#  include "chip//stm32f30xxx_vectors.h"
#elif defined(CHIP_STM32F37XX)
#  include "chip//stm32f37xxx_vectors.h"
#elif defined(CHIP_STM32F40XX)
#  include "chip//stm32f40xxx_vectors.h"
#else
#  error "No handlers for STM32 chip"
#endif

#endif /* CHIP_NOEXT_VECTORS */


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

	bl do_irq
	
	pop {pc}
