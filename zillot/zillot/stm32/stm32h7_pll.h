#ifndef ZILLOT_STM32_STM32H7_PLL_H
#define ZILLOT_STM32_STM32H7_PLL_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <zillot/stm32/stm32_rcc.h>
#include <igris/util/bits.h>

__BEGIN_DECLS

static inline void stm32h7_set_pll1_source_hse() 
{
    bits_assign(RCC->PLLCKSELR,
        RCC_PLLCKSELR_PLLSRC_Msk, RCC_PLLCKSELR_PLLSRC_HSE);
}

static inline void stm32h7_set_pll1_coefficients(
    unsigned int m,
    unsigned int n,
    unsigned int p) 
{    
        bits_assign(RCC->PLLCKSELR,
            RCC_PLLCKSELR_DIVM1_Msk, ((m) << RCC_PLLCKSELR_DIVM1_Pos));
    
        bits_assign(RCC->PLL1DIVR,
            RCC_PLL1DIVR_N1_Msk, ((n-1) << RCC_PLL1DIVR_N1_Pos));

        bits_assign(RCC->PLL1DIVR,
            RCC_PLL1DIVR_P1_Msk, ((p-1) << RCC_PLL1DIVR_P1_Pos));
}

__END_DECLS

#endif