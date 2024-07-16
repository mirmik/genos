#include <igris/util/bits.h>
#include <stm32xxxx.h>
#include <zillot/stm32/stm32_clockbus.h>
#include <zillot/stm32/stm32_systick.h>

static int _sysclk_source = 0;

void clockbus_set_sysclk_source(clockbus_clock_e clock)
{
    _sysclk_source = clock;
}

clockbus_clock_e clockbus_sysclk_source()
{
    return _sysclk_source;
}

void stm32_clockbus_enable_hsi_mode()
{
    volatile int i;
    RCC->CR = RCC_CR_HSION;
    i = 1000;
    while (--i)
        ;
}

#ifdef RCC_CFGR_HPRE_DIV1
uint32_t hpre_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_CFGR_HPRE_DIV1;
    case 2:
        return RCC_CFGR_HPRE_DIV2;
    case 4:
        return RCC_CFGR_HPRE_DIV4;
    case 8:
        return RCC_CFGR_HPRE_DIV8;
    case 16:
        return RCC_CFGR_HPRE_DIV16;
    case 64:
        return RCC_CFGR_HPRE_DIV64;
    case 128:
        return RCC_CFGR_HPRE_DIV128;
    case 256:
        return RCC_CFGR_HPRE_DIV256;
    case 512:
        return RCC_CFGR_HPRE_DIV512;
    }
    return 0;
}
#endif

#ifdef RCC_D1CFGR_HPRE
uint32_t hpre_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_D1CFGR_HPRE_DIV1;
    case 2:
        return RCC_D1CFGR_HPRE_DIV2;
    case 4:
        return RCC_D1CFGR_HPRE_DIV4;
    case 8:
        return RCC_D1CFGR_HPRE_DIV8;
    case 16:
        return RCC_D1CFGR_HPRE_DIV16;
    case 64:
        return RCC_D1CFGR_HPRE_DIV64;
    case 128:
        return RCC_D1CFGR_HPRE_DIV128;
    case 256:
        return RCC_D1CFGR_HPRE_DIV256;
    case 512:
        return RCC_D1CFGR_HPRE_DIV512;
    }
    return 0;
}
#endif

#ifdef RCC_CFGR_PPRE1_DIV1
uint32_t ppre1_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_CFGR_PPRE1_DIV1;
    case 2:
        return RCC_CFGR_PPRE1_DIV2;
    case 4:
        return RCC_CFGR_PPRE1_DIV4;
    case 8:
        return RCC_CFGR_PPRE1_DIV8;
    case 16:
        return RCC_CFGR_PPRE1_DIV16;
    }
    return 0;
}
#endif

#ifdef RCC_CFGR_PPRE2_DIV1
uint32_t ppre2_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_CFGR_PPRE2_DIV1;
    case 2:
        return RCC_CFGR_PPRE2_DIV2;
    case 4:
        return RCC_CFGR_PPRE2_DIV4;
    case 8:
        return RCC_CFGR_PPRE2_DIV8;
    case 16:
        return RCC_CFGR_PPRE2_DIV16;
    }
    return 0;
}
#endif

#ifdef RCC_CFGR_HPRE_DIV1
uint16_t hpre_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_CFGR_HPRE_DIV1:
        return 1;
    case RCC_CFGR_HPRE_DIV2:
        return 2;
    case RCC_CFGR_HPRE_DIV4:
        return 4;
    case RCC_CFGR_HPRE_DIV8:
        return 8;
    case RCC_CFGR_HPRE_DIV16:
        return 16;
    case RCC_CFGR_HPRE_DIV64:
        return 64;
    case RCC_CFGR_HPRE_DIV128:
        return 128;
    case RCC_CFGR_HPRE_DIV256:
        return 256;
    case RCC_CFGR_HPRE_DIV512:
        return 512;
    }
    return 0;
}
#endif

#ifdef RCC_CFGR_PPRE1_DIV1
uint16_t ppre1_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_CFGR_PPRE1_DIV1:
        return 1;
    case RCC_CFGR_PPRE1_DIV2:
        return 2;
    case RCC_CFGR_PPRE1_DIV4:
        return 4;
    case RCC_CFGR_PPRE1_DIV8:
        return 8;
    case RCC_CFGR_PPRE1_DIV16:
        return 16;
    }
    return 0;
}
#endif

#ifdef RCC_CFGR_PPRE2_DIV1
uint16_t ppre2_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_CFGR_PPRE2_DIV1:
        return 1;
    case RCC_CFGR_PPRE2_DIV2:
        return 2;
    case RCC_CFGR_PPRE2_DIV4:
        return 4;
    case RCC_CFGR_PPRE2_DIV8:
        return 8;
    case RCC_CFGR_PPRE2_DIV16:
        return 16;
    }
    return 0;
}
#endif

#ifdef RCC_D1CFGR_HPRE
uint16_t hpre_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_D1CFGR_HPRE_DIV1:
        return 1;
    case RCC_D1CFGR_HPRE_DIV2:
        return 2;
    case RCC_D1CFGR_HPRE_DIV4:
        return 4;
    case RCC_D1CFGR_HPRE_DIV8:
        return 8;
    case RCC_D1CFGR_HPRE_DIV16:
        return 16;
    case RCC_D1CFGR_HPRE_DIV64:
        return 64;
    case RCC_D1CFGR_HPRE_DIV128:
        return 128;
    case RCC_D1CFGR_HPRE_DIV256:
        return 256;
    case RCC_D1CFGR_HPRE_DIV512:
        return 512;
    }
    return 0;
}
#endif

#ifdef RCC_CFGR_HPRE
void stm32_clockbus_set_hpre_divider(int divider)
{
    uint32_t value = hpre_divider_value(divider);
    bits_assign(RCC->CFGR, RCC_CFGR_HPRE, value);
}
#endif

#ifdef RCC_D1CFGR_HPRE
void stm32_clockbus_set_hpre_divider(int divider)
{
    uint32_t value = hpre_divider_value(divider);
    bits_assign(RCC->D1CFGR, RCC_D1CFGR_HPRE, value);
}
#endif

#ifdef RCC_CFGR_HPRE
uint16_t stm32_clockbus_get_hpre_divider()
{
    uint32_t value = bits_mask(RCC->CFGR, RCC_CFGR_HPRE);
    return hpre_value_to_divider(value);
}
#endif

#ifdef RCC_CFGR_PPRE1
void stm32_clockbus_set_ppre1_divider(int divider)
{
    uint32_t value = ppre1_divider_value(divider);
    bits_assign(RCC->CFGR, RCC_CFGR_PPRE1, value);
}
#endif

#ifdef RCC_CFGR_PPRE1
uint16_t stm32_clockbus_get_ppre1_divider()
{
    uint32_t value = bits_mask(RCC->CFGR, RCC_CFGR_PPRE1);
    return ppre1_value_to_divider(value);
}
#endif

#ifdef RCC_D1CFGR_HPRE
uint16_t stm32_clockbus_get_hpre_divider()
{
    uint32_t value = bits_mask(RCC->D1CFGR, RCC_D1CFGR_HPRE);
    return hpre_value_to_divider(value);
}
#endif

#ifdef RCC_D1CFGR_D1CPRE
uint16_t d1cpre_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_D1CFGR_D1CPRE_DIV1:
        return 1;
    case RCC_D1CFGR_D1CPRE_DIV2:
        return 2;
    case RCC_D1CFGR_D1CPRE_DIV4:
        return 4;
    case RCC_D1CFGR_D1CPRE_DIV8:
        return 8;
    case RCC_D1CFGR_D1CPRE_DIV16:
        return 16;
    }
    return 0;
}
uint32_t d1cpre_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_D1CFGR_D1CPRE_DIV1;
    case 2:
        return RCC_D1CFGR_D1CPRE_DIV2;
    case 4:
        return RCC_D1CFGR_D1CPRE_DIV4;
    case 8:
        return RCC_D1CFGR_D1CPRE_DIV8;
    case 16:
        return RCC_D1CFGR_D1CPRE_DIV16;
    }
    return 0;
}
#endif

#ifdef RCC_D1CFGR_D1CPRE
uint16_t stm32_clockbus_get_d1cpre_divider()
{
    uint32_t value = bits_mask(RCC->CFGR, RCC_D1CFGR_D1CPRE);
    return d1cpre_value_to_divider(value);
}
void stm32_clockbus_set_d1cpre_divider(int divider)
{
    uint32_t value = d1cpre_divider_value(divider);
    bits_assign(RCC->D1CFGR, RCC_D1CFGR_D1CPRE, value);
}
#endif

#ifdef RCC_CFGR_PPRE2
void stm32_clockbus_set_ppre2_divider(int divider)
{
    uint32_t value = ppre2_divider_value(divider);
    bits_assign(RCC->CFGR, RCC_CFGR_PPRE2, value);
}
uint16_t stm32_clockbus_get_ppre2_divider()
{
    uint32_t value = bits_mask(RCC->CFGR, RCC_CFGR_PPRE2);
    return ppre2_value_to_divider(value);
}
#endif

#ifdef RCC_D1CFGR_D1PPRE
uint32_t d1ppre_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_D1CFGR_D1PPRE_DIV1;
    case 2:
        return RCC_D1CFGR_D1PPRE_DIV2;
    case 4:
        return RCC_D1CFGR_D1PPRE_DIV4;
    case 8:
        return RCC_D1CFGR_D1PPRE_DIV8;
    case 16:
        return RCC_D1CFGR_D1PPRE_DIV16;
    }
    return 0;
}
uint16_t d1ppre_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_D1CFGR_D1PPRE_DIV1:
        return 1;
    case RCC_D1CFGR_D1PPRE_DIV2:
        return 2;
    case RCC_D1CFGR_D1PPRE_DIV4:
        return 4;
    case RCC_D1CFGR_D1PPRE_DIV8:
        return 8;
    case RCC_D1CFGR_D1PPRE_DIV16:
        return 16;
    }
    return 0;
}
#endif

#ifdef RCC_D2CFGR_D2PPRE2
uint32_t d2ppre2_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_D2CFGR_D2PPRE2_DIV1;
    case 2:
        return RCC_D2CFGR_D2PPRE2_DIV2;
    case 4:
        return RCC_D2CFGR_D2PPRE2_DIV4;
    case 8:
        return RCC_D2CFGR_D2PPRE2_DIV8;
    case 16:
        return RCC_D2CFGR_D2PPRE2_DIV16;
    }
    return 0;
}
uint16_t d2ppre2_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_D2CFGR_D2PPRE2_DIV1:
        return 1;
    case RCC_D2CFGR_D2PPRE2_DIV2:
        return 2;
    case RCC_D2CFGR_D2PPRE2_DIV4:
        return 4;
    case RCC_D2CFGR_D2PPRE2_DIV8:
        return 8;
    case RCC_D2CFGR_D2PPRE2_DIV16:
        return 16;
    }
    return 0;
}
#endif

#ifdef RCC_D2CFGR_D2PPRE1
uint32_t d2ppre1_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_D2CFGR_D2PPRE1_DIV1;
    case 2:
        return RCC_D2CFGR_D2PPRE1_DIV2;
    case 4:
        return RCC_D2CFGR_D2PPRE1_DIV4;
    case 8:
        return RCC_D2CFGR_D2PPRE1_DIV8;
    case 16:
        return RCC_D2CFGR_D2PPRE1_DIV16;
    }
    return 0;
}
#endif

#ifdef RCC_D2CFGR_D2PPRE1
uint16_t d2ppre1_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_D2CFGR_D2PPRE1_DIV1:
        return 1;
    case RCC_D2CFGR_D2PPRE1_DIV2:
        return 2;
    case RCC_D2CFGR_D2PPRE1_DIV4:
        return 4;
    case RCC_D2CFGR_D2PPRE1_DIV8:
        return 8;
    case RCC_D2CFGR_D2PPRE1_DIV16:
        return 16;
    }
    return 0;
}
#endif

#ifdef RCC_D3CFGR_D3PPRE
uint32_t d3ppre_divider_value(uint16_t divider)
{
    switch (divider)
    {
    case 1:
        return RCC_D3CFGR_D3PPRE_DIV1;
    case 2:
        return RCC_D3CFGR_D3PPRE_DIV2;
    case 4:
        return RCC_D3CFGR_D3PPRE_DIV4;
    case 8:
        return RCC_D3CFGR_D3PPRE_DIV8;
    case 16:
        return RCC_D3CFGR_D3PPRE_DIV16;
    }
    return 0;
}
#endif

#ifdef RCC_D3CFGR_D3PPRE
uint16_t d3ppre_value_to_divider(uint32_t value)
{
    switch (value)
    {
    case RCC_D3CFGR_D3PPRE_DIV1:
        return 1;
    case RCC_D3CFGR_D3PPRE_DIV2:
        return 2;
    case RCC_D3CFGR_D3PPRE_DIV4:
        return 4;
    case RCC_D3CFGR_D3PPRE_DIV8:
        return 8;
    case RCC_D3CFGR_D3PPRE_DIV16:
        return 16;
    }
    return 0;
}
#endif

#ifdef RCC_D1CFGR_D1PPRE
void stm32_clockbus_set_d1ppre_divider(int divider)
{
    uint32_t value = d1ppre_divider_value(divider);
    bits_assign(RCC->D1CFGR, RCC_D1CFGR_D1PPRE, value);
}
uint16_t stm32_clockbus_get_d1ppre_divider()
{
    uint32_t value = bits_mask(RCC->D1CFGR, RCC_D1CFGR_D1PPRE);
    return d1ppre_value_to_divider(value);
}
#endif

#ifdef RCC_D2CFGR_D2PPRE2
void stm32_clockbus_set_d2ppre2_divider(int divider)
{
    uint32_t value = d2ppre2_divider_value(divider);
    bits_assign(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2, value);
}
uint16_t stm32_clockbus_get_d2ppre2_divider()
{
    uint32_t value = bits_mask(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2);
    return d2ppre2_value_to_divider(value);
}
#endif

#ifdef RCC_D2CFGR_D2PPRE1
void stm32_clockbus_set_d2ppre1_divider(int divider)
{
    uint32_t value = d2ppre1_divider_value(divider);
    bits_assign(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1, value);
}
uint16_t stm32_clockbus_get_d2ppre1_divider()
{
    uint32_t value = bits_mask(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1);
    return d2ppre1_value_to_divider(value);
}
#endif

#ifdef RCC_D3CFGR_D3PPRE
void stm32_clockbus_set_d3ppre_divider(int divider)
{
    uint32_t value = d3ppre_divider_value(divider);
    bits_assign(RCC->D3CFGR, RCC_D3CFGR_D3PPRE, value);
}
uint16_t stm32_clockbus_get_d3ppre_divider()
{
    uint32_t value = bits_mask(RCC->D3CFGR, RCC_D3CFGR_D3PPRE);
    return d3ppre_value_to_divider(value);
}
#endif

#ifdef STM32H7XX
void clockbus_reeval_clocks_h7()
{
    uint8_t sysclk_source = clockbus_sysclk_source();
    switch (sysclk_source)
    {
    case CLOCKBUS_HSI:
        stm32_clockbus_freq[CLOCKBUS_SYSCLK] =
            stm32_clockbus_freq[CLOCKBUS_HSI];
        break;
    case CLOCKBUS_HSE:
        stm32_clockbus_freq[CLOCKBUS_SYSCLK] =
            stm32_clockbus_freq[CLOCKBUS_HSE];
        break;
    case CLOCKBUS_PLL:
        stm32_clockbus_freq[CLOCKBUS_SYSCLK] =
            stm32_clockbus_freq[CLOCKBUS_PLL];
        break;
    }

    stm32_clockbus_freq[CLOCKBUS_D1CPRE] =
        stm32_clockbus_freq[CLOCKBUS_SYSCLK] /
        stm32_clockbus_get_d1cpre_divider();

    stm32_clockbus_freq[CLOCKBUS_HCLK] = stm32_clockbus_freq[CLOCKBUS_D1CPRE] /
                                         stm32_clockbus_get_hpre_divider();

    stm32_clockbus_freq[CLOCKBUS_APB1] = stm32_clockbus_freq[CLOCKBUS_HCLK] /
                                         stm32_clockbus_get_d2ppre1_divider();

    stm32_clockbus_freq[CLOCKBUS_APB2] = stm32_clockbus_freq[CLOCKBUS_HCLK] /
                                         stm32_clockbus_get_d2ppre2_divider();

    stm32_clockbus_freq[CLOCKBUS_SYSTICK] =
        stm32_clockbus_freq[CLOCKBUS_D1CPRE];
}
#endif

#ifdef STM32F4XX
void clockbus_reeval_clocks_f4() {}
#endif

void clockbus_reeval_clocks()
{
#ifdef STM32H7XX
    clockbus_reeval_clocks_h7();
#endif

#ifdef STM32F4XX
    clockbus_reeval_clocks_f4();
#endif
}