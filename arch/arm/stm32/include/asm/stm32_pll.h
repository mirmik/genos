#ifndef GENOS_ARCH_ARM_STM32_PLL_H
#define GENOS_ARCH_ARM_STM32_PLL_H

#include <stdint.h>
#include <sys/cdefs.h>

#if defined(STM32G4XX) || defined(STM32L4XX) 
#define STM32_PLL_VAR1
struct stm32_pll_settings
{
	uint32_t Mkoeff;
	uint32_t Nkoeff;
	uint32_t Rkoeff;
	uint32_t Pkoeff;
	uint32_t Qkoeff;
};
#else
#define STM32_PLL_VAR2
struct stm32_pll_settings
{
	uint32_t Mkoeff;
	uint32_t Nkoeff;
	uint32_t Pkoeff;
	uint32_t Qkoeff;
};
#endif

__BEGIN_DECLS

unsigned stm32_init_pll_clocking(struct stm32_pll_settings*);
int stm32_clockbus_enable_pll_mode(uint32_t freq);

#if defined(STM32_PLL_VAR1)
int stm32_pll_setup(int M, int N, int R, int P, int Q);
#elif defined(STM32_PLL_VAR2)
int stm32_pll_setup(int M, int N, int P, int Q);
#endif
__END_DECLS

#endif