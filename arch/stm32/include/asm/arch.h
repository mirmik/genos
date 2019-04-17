#ifndef STM32_ARCH_H
#define STM32_ARCH_H

#include <sys/cdefs.h>
#include <asm/chip.h>

#include <periph/regs/usart.h>

__BEGIN_DECLS

void arch_init();
void stm32_diag_init(struct usart_regs*);

struct stm32_pll_settings
{
	uint32_t Mkoeff;
	uint32_t Nkoeff;
	uint32_t Pkoeff;
	uint32_t Qkoeff;
};

unsigned stm32_init_pll_clocking(struct stm32_pll_settings*);
int stm32_systick_config(uint32_t ticks);

__END_DECLS

#endif /* HAL_ARCH_H_ */
