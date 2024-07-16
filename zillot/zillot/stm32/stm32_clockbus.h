#ifndef STM32_CLOCKBUS_H
#define STM32_CLOCKBUS_H

#include <igris/compiler.h>
#include <stdint.h>
#include <stm32xxxx.h>

enum clockbus_clock_enum
{
    CLOCKBUS_HSI,
    CLOCKBUS_HSE,
    CLOCKBUS_MSI,
    CLOCKBUS_PLL,
    CLOCKBUS_PLL2,
    CLOCKBUS_PLL3,
    CLOCKBUS_SYSCLK,
    CLOCKBUS_SYSTICK,
    CLOCKBUS_HCLK,
    CLOCKBUS_D1CPRE,
    CLOCKBUS_D1PPRE,
    CLOCKBUS_APB1,
    CLOCKBUS_APB2,
    CLOCKBUS_D3PPRE,
    CLOCKBUS_CLOCK_MAX,
};
typedef enum clockbus_clock_enum clockbus_clock_e;

enum clockbus_divider_enum
{
#ifdef RCC_D1CFGR_D1CPRE
    CLOCKBUS_D1CPRE_DIVIDER,
#endif

    CLOCKBUS_HPRE_DIVIDER,

#ifdef RCC_CFGR_PPRE1
    CLOCKBUS_PPRE1_DIVIDER,
#endif
#ifdef RCC_CFGR_PPRE2
    CLOCKBUS_PPRE2_DIVIDER,
#endif

#ifdef RCC_D1CFGR_D1PPRE
    CLOCKBUS_D1PPRE_DIVIDER,
#endif

#ifdef RCC_D2CFGR_D2PPRE1
    CLOCKBUS_D2PPRE1_DIVIDER,
#endif

#ifdef RCC_D2CFGR_D2PPRE2
    CLOCKBUS_D2PPRE2_DIVIDER,
#endif

#ifdef RCC_D3CFGR_D3PPRE
    CLOCKBUS_D3PPRE_DIVIDER
#endif
};
typedef enum clockbus_divider_enum clockbus_divider_e;

extern uint32_t stm32_clockbus_freq[(int)CLOCKBUS_CLOCK_MAX];

__BEGIN_DECLS

void clockbus_set_sysclk_source(clockbus_clock_e clock);
clockbus_clock_e clockbus_sysclk_source();
void clockbus_reeval_clocks();

void stm32_clockbus_enable_hsi_mode();

// void stm32_clockbus_hsi_mode();
// void stm32_clockbus_hse_mode();
// void stm32_clockbus_systime_setup();

void stm32_clockbus_set_hpre_divider(int divider);
uint16_t stm32_clockbus_get_hpre_divider();
void stm32_clockbus_set_ppre1_divider(int divider);
uint16_t stm32_clockbus_get_ppre1_divider();
void stm32_clockbus_set_ppre2_divider(int divider);
uint16_t stm32_clockbus_get_ppre2_divider();

void stm32_clockbus_set_d1cpre_divider(int divider);
uint16_t stm32_clockbus_get_d1cpre_divider();

void stm32_clockbus_set_d1ppre_divider(int divider);
uint16_t stm32_clockbus_get_d1ppre_divider();
void stm32_clockbus_set_d2ppre2_divider(int divider);
uint16_t stm32_clockbus_get_d2ppre2_divider();
void stm32_clockbus_set_d2ppre1_divider(int divider);
uint16_t stm32_clockbus_get_d2ppre1_divider();
void stm32_clockbus_set_d3ppre_divider(int divider);
uint16_t stm32_clockbus_get_d3ppre_divider();

uint32_t stm32_sysclk_clock_freq();
uint32_t stm32_d1cpre_clock_freq();
uint32_t stm32_hclk_clock_freq();
uint32_t stm32_cpu_clock_freq();

__END_DECLS

#endif