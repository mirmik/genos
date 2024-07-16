#ifndef ASM_STM32_RCC_H
#define ASM_STM32_RCC_H

#include <igris/compiler.h>
#include <stm32xxxx.h>
#include <zillot/stm32/stm32_clockbus.h>

__BEGIN_DECLS

void stm32_rcc_enable_gpio(GPIO_TypeDef *gpio);
void stm32_rcc_enable_usart(USART_TypeDef *usart);
void stm32_rcc_enable_spi(SPI_TypeDef *spi);
void stm32_rcc_enable_i2c(I2C_TypeDef *spi);
void stm32_rcc_enable_adc(ADC_TypeDef *spi);
void stm32_rcc_enable_timer(TIM_TypeDef *spi);
void stm32_rcc_enable_syscfg();

static inline void stm32_enable_hse()
{
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY))
        ;
}

static inline void stm32_enable_pll()
{
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;
}

static inline void stm32_disable_pll()
{
    RCC->CR &= ~(RCC_CR_PLLON);
    while (RCC->CR & RCC_CR_PLLRDY)
        ;
}

static inline void stm32_system_clock_mux_hse()
{
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= (RCC_CFGR_SW_HSE);
    while ((RCC->CFGR & RCC_CFGR_SWS_HSE) != RCC_CFGR_SWS_HSE)
        ;
    clockbus_set_sysclk_source(CLOCKBUS_HSE);
}

static inline void stm32_system_clock_mux_hsi()
{
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= (RCC_CFGR_SW_HSI);
    while ((RCC->CFGR & RCC_CFGR_SWS_HSI) != RCC_CFGR_SWS_HSI)
        ;
    clockbus_set_sysclk_source(CLOCKBUS_HSI);
}

static inline int stm32_system_clock_mux_pll(int pllno)
{
#ifdef RCC_CFGR_SW_PLL
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= (RCC_CFGR_SW_PLL);
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL)
        ;
    clockbus_set_sysclk_source(CLOCKBUS_PLL);
    return 0;
#else

    switch (pllno)
    {
#ifdef RCC_CFGR_SW_PLL1
    case 0:
        RCC->CFGR &= ~(RCC_CFGR_SW);
        RCC->CFGR |= (RCC_CFGR_SW_PLL1);
        while ((RCC->CFGR & RCC_CFGR_SWS_PLL1) != RCC_CFGR_SWS_PLL1)
            ;
        clockbus_set_sysclk_source(CLOCKBUS_PLL);
        return 0;
#endif
#ifdef RCC_CFGR_SW_PLL2
    case 1:
        RCC->CFGR &= ~(RCC_CFGR_SW);
        RCC->CFGR |= (RCC_CFGR_SW_PLL2);
        while ((RCC->CFGR & RCC_CFGR_SWS_PLL2) != RCC_CFGR_SWS_PLL2)
            ;
        clockbus_set_sysclk_source(CLOCKBUS_PLL2);
        return 0;
#endif
#ifdef RCC_CFGR_SW_PLL3
    case 2:
        RCC->CFGR &= ~(RCC_CFGR_SW);
        RCC->CFGR |= (RCC_CFGR_SW_PLL3);
        while ((RCC->CFGR & RCC_CFGR_SWS_PLL3) != RCC_CFGR_SWS_PLL3)
            ;
        clockbus_set_sysclk_source(CLOCKBUS_PLL3);
        return 0;
#endif
    default:
        return -1;
    }

#endif
}

void stm32_set_flash_latency(int latency);
void stm32_enable_icache();
void stm32_enable_dcache();

__END_DECLS

#endif