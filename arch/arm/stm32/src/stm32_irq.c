#include "periph/irqdefs.h"
#include <assert.h>
#include <kernel/panic.h>

#include <asm/chip.h>
#include <armv7-m/nvic.h>

#include <regops.h>

#define DEFPRIORITY32 \
	(NVIC_SYSH_PRIORITY_DEFAULT << 24 | \
	 NVIC_SYSH_PRIORITY_DEFAULT << 16 | \
	 NVIC_SYSH_PRIORITY_DEFAULT << 8  | \
	 NVIC_SYSH_PRIORITY_DEFAULT)

#define NVIC_ENA_OFFSET    (0)
#define NVIC_CLRENA_OFFSET (NVIC_IRQ0_31_CLEAR - NVIC_IRQ0_31_ENABLE)

static int stm32_irqinfo(int irq, uintptr_t *regaddr, uint32_t *bit, uintptr_t offset) {
	assert(irq >= STM32_IRQ_NMI && irq < NR_IRQS);

	/* Check for external interrupt */

	if (irq >= STM32_IRQ_FIRST) {
		if (irq < STM32_IRQ_FIRST + 32) {
			*regaddr = (NVIC_IRQ0_31_ENABLE + offset);
			*bit     = 1 << (irq - STM32_IRQ_FIRST);
		}
		else if (irq < STM32_IRQ_FIRST + 64) {
			*regaddr = (NVIC_IRQ32_63_ENABLE + offset);
			*bit     = 1 << (irq - STM32_IRQ_FIRST - 32);
		}
		else if (irq < NR_IRQS) {
			*regaddr = (NVIC_IRQ64_95_ENABLE + offset);
			*bit     = 1 << (irq - STM32_IRQ_FIRST - 64);
		}
		else {
			return 1; /* Invalid interrupt */
		}
	}

	/* Handle processor exceptions.  Only a few can be disabled */

	else {
		*regaddr = NVIC_SYSHCON;
		if (irq == STM32_IRQ_MEMFAULT) {
			*bit = NVIC_SYSHCON_MEMFAULTENA;
		}
		else if (irq == STM32_IRQ_BUSFAULT) {
			*bit = NVIC_SYSHCON_BUSFAULTENA;
		}
		else if (irq == STM32_IRQ_USAGEFAULT) {
			*bit = NVIC_SYSHCON_USGFAULTENA;
		}
		else if (irq == STM32_IRQ_SYSTICK) {
			*regaddr = NVIC_SYSTICK_CTRL;
			*bit = NVIC_SYSTICK_CTRL_ENABLE;
		}
		else {
			return 1; /* Invalid or unsupported exception */
		}
	}
	return 0;
}

void irq_enable(int irq) {
	uintptr_t regaddr;
	uint32_t regval;
	uint32_t bit;

	if (stm32_irqinfo(irq, &regaddr, &bit, NVIC_ENA_OFFSET) == 0)
	{
	  /* Modify the appropriate bit in the register to enable the interrupt.
	   * For normal interrupts, we need to set the bit in the associated
	   * Interrupt Set Enable register.  For other exceptions, we need to
	   * set the bit in the System Handler Control and State Register.
	   */

	  if (irq >= STM32_IRQ_FIRST)
		{
		  putreg32(bit, regaddr);
		}
	  else
		{
		  regval  = getreg32(regaddr);
		  regval |= bit;
		  putreg32(regval, regaddr);
		}
	}
}