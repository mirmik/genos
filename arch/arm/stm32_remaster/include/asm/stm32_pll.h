#ifndef GENOS_ARCH_ARM_STM32_PLL_H
#define GENOS_ARCH_ARM_STM32_PLL_H

#include <stdint.h>
#include <sys/cdefs.h>

struct stm32_pll_settings
{
	uint32_t Mkoeff;
	uint32_t Nkoeff;
	uint32_t Rkoeff;
	uint32_t Pkoeff;
	uint32_t Qkoeff;
};

__BEGIN_DECLS

unsigned stm32_init_pll_clocking(struct stm32_pll_settings*);
int stm32_pll_setup(int M, int N, int R, int P, int Q);
int stm32_clockbus_enable_pll_mode(uint32_t freq);

__END_DECLS

#endif