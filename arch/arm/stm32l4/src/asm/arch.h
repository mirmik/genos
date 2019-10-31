#ifndef STM32L4_ARCH_H
#define STM32L4_ARCH_H

#include <sys/cdefs.h>
#include <periph/map.h>

extern int32_t stm32_declared_clockbus_freq[3];

#define CLOCKBUS_NO_PLL 0
#define CLOCKBUS_NO_APB1 0
#define CLOCKBUS_NO_APB2 0

__BEGIN_DECLS

struct stm32l4_pll_settings
{
	uint32_t Mkoeff;
	uint32_t Nkoeff;
	uint32_t Pkoeff;
	uint32_t Rkoeff;
	uint32_t Qkoeff;
};

unsigned stm32l4_init_pll_clocking(struct stm32l4_pll_settings*);

void stm32l4_clock_source_hse();

int stm32_systick_config(uint32_t ticks);
void stm32l4_diag_init(usart_regs_t* regs);

__END_DECLS

#endif