#include <igris/util/bug.h>
#include <stm32xxxx.h>
#include <zillot/stm32/stm32_rcc.h>

uint8_t _sysclk_source;
uint32_t stm32_clockbus_freq[(int)CLOCKBUS_CLOCK_MAX];

void stm32_rcc_enable_gpio(GPIO_TypeDef *gpio)
{
    switch ((uintptr_t)gpio)
    {
#ifdef RCC_AHB1ENR_GPIOAEN
    case GPIOA_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        break;
#endif

#ifdef RCC_AHB2ENR_GPIOAEN
    case GPIOA_BASE:
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
        break;
#endif

#ifdef RCC_AHB4ENR_GPIOAEN
    case GPIOA_BASE:
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIOBEN
    case GPIOB_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        break;
#endif

#ifdef RCC_AHB2ENR_GPIOBEN
    case GPIOB_BASE:
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
        break;
#endif

#ifdef RCC_AHB4ENR_GPIOBEN
    case GPIOB_BASE:
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIOCEN
    case GPIOC_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
        break;
#endif

#ifdef RCC_AHB2ENR_GPIOCEN
    case GPIOC_BASE:
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
        break;
#endif

#ifdef RCC_AHB4ENR_GPIOCEN
    case GPIOC_BASE:
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIODEN
    case GPIOD_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
        break;
#endif

#ifdef RCC_AHB4ENR_GPIODEN
    case GPIOD_BASE:
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIODEN;
        break;
#endif

#ifdef RCC_AHB2ENR_GPIODEN
    case GPIOD_BASE:
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIOEEN
    case GPIOE_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
        break;
#endif

#ifdef RCC_AHB4ENR_GPIOEEN
    case GPIOE_BASE:
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOEEN;
        break;
#endif

#ifdef RCC_AHB2ENR_GPIOFEN
    case GPIOF_BASE:
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIOFEN
    case GPIOF_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
        break;
#endif

#ifdef RCC_AHB4ENR_GPIOFEN
    case GPIOF_BASE:
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOFEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIOGEN
    case GPIOG_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
        break;
#endif

#ifdef RCC_AHB4ENR_GPIOGEN
    case GPIOG_BASE:
        RCC->AHB4ENR |= RCC_AHB4ENR_GPIOGEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIOHEN
    case GPIOH_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
        break;
#endif

#ifdef RCC_AHB2ENR_GPIOHEN
    case GPIOH_BASE:
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
        break;
#endif

#ifdef RCC_AHB1ENR_GPIOIEN
    case GPIOI_BASE:
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
        break;
#endif

    default:
        BUG();
    }
}

void stm32_rcc_enable_usart(USART_TypeDef *usart)
{
    switch ((uintptr_t)usart)
    {
#ifdef RCC_APB1ENR_USART2EN
    case USART2_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        break;
#elif defined(RCC_APB1ENR1_USART2EN)
    case USART2_BASE:
        RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
        break;
#endif

#ifdef RCC_APB2ENR_USART6EN
    case USART6_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        break;
#endif

#ifdef RCC_APB1ENR2_LPUART1EN
    case LPUART1_BASE:
        RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
        break;
#endif

#ifdef RCC_APB2ENR_USART1EN
    case USART1_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        break;
#endif

#ifdef RCC_APB1ENR_USART3EN
    case USART3_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        break;
#endif

#ifdef RCC_APB1LENR_UART8EN
    case UART8_BASE:
        RCC->APB1LENR |= RCC_APB1LENR_UART8EN;
        break;
#endif

#ifdef RCC_APB2ENR_USART10EN
    case USART10_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_USART10EN;
        break;
#endif

#ifdef RCC_APB1ENR_UART5EN
    case UART5_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
        break;
#endif

    default:
        BUG();
    }
}

void stm32_rcc_enable_timer(TIM_TypeDef *t)
{
    switch ((uintptr_t)t)
    {
    case TIM1_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
        break;

#ifdef RCC_APB1ENR_TIM2EN
    case TIM2_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
        break;
#endif

#ifdef RCC_APB1ENR1_TIM2EN
    case TIM2_BASE:
        RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
        break;
#endif

#ifdef RCC_APB1LENR_TIM2EN
    case TIM2_BASE:
        RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
        break;
#endif

#ifdef RCC_APB1LENR_TIM3EN
    case TIM3_BASE:
        RCC->APB1LENR |= RCC_APB1LENR_TIM3EN;
        break;
#endif

#ifdef RCC_APB1ENR_TIM3EN
    case TIM3_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
        break;
#endif

#ifdef RCC_APB1ENR_TIM4EN
    case TIM4_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
        break;
#endif

#ifdef RCC_APB1ENR1_TIM4EN
    case TIM4_BASE:
        RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
        break;
#endif

#ifdef RCC_APB1LENR_TIM4EN
    case TIM4_BASE:
        RCC->APB1LENR |= RCC_APB1LENR_TIM4EN;
        break;
#endif

#ifdef RCC_APB1ENR_TIM5EN
    case TIM5_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
        break;
#endif

#ifdef RCC_APB1ENR1_TIM5EN
    case TIM5_BASE:
        RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
        break;
#endif

#ifdef RCC_APB1LENR_TIM5EN
    case TIM5_BASE:
        RCC->APB1LENR |= RCC_APB1LENR_TIM5EN;
        break;
#endif

#ifdef TIM6_BASE
#ifdef RCC_APB1ENR_TIM6EN
    case TIM6_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
        break;
#endif
#endif

#ifdef TIM7_BASE
#ifdef RCC_APB1ENR_TIM7EN
    case TIM7_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
        break;
#endif
#endif

#ifdef TIM8_BASE
    case TIM8_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
        break;
#endif

#ifdef TIM9_BASE
    case TIM9_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
        break;
#endif

#ifdef TIM10_BASE
    case TIM10_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
        break;
#endif

#ifdef TIM11_BASE
    case TIM11_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
        break;
#endif

#ifdef TIM12_BASE
#ifdef RCC_APB1ENR_TIM12EN
    case TIM12_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
        break;
#endif
#endif

#ifdef TIM13_BASE
#ifdef RCC_APB1ENR_TIM13EN
    case TIM13_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
        break;
#endif
#endif

#ifdef RCC_APB1ENR_TIM14EN
    case TIM14_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
        break;
#endif

    default:
        BUG();
    }
}

void stm32_rcc_enable_adc(ADC_TypeDef *regs)
{
    switch ((uintptr_t)regs)
    {
#ifdef RCC_APB2ENR_ADC1EN
    case ADC1_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
        break;
#endif

#ifdef RCC_APB2ENR_ADC2EN
    case ADC2_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
        break;
#endif

#ifdef RCC_APB2ENR_ADC3EN
    case ADC3_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
        break;
#endif

    default:
        BUG();
    }
}

void stm32_rcc_enable_i2c(I2C_TypeDef *regs)
{
    switch ((uintptr_t)regs)
    {
#ifdef RCC_APB1ENR_I2C1EN
    case I2C1_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
        break;
#endif

    default:
        BUG();
    }
}

void stm32_rcc_enable_spi(SPI_TypeDef *regs)
{
    switch ((uintptr_t)regs)
    {
#ifdef RCC_APB2ENR_SPI1EN
    case SPI1_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        break;
#endif

#ifdef RCC_APB1ENR_SPI2EN
    case SPI2_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
        break;
#endif

#ifdef RCC_APB1ENR_SPI3EN
    case SPI3_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
        break;
#endif

#ifdef RCC_APB2ENR_SPI4EN
    case SPI4_BASE:
        RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
        break;
#endif

    default:
        BUG();
    }
}

void stm32_rcc_enable_syscfg()
{
#if defined(RCC_APB2ENR_SYSCFGEN)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
#elif defined(RCC_APB4ENR_SYSCFGEN)
    RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;
#else
#error "SysCfg enable register is not supported"
#endif
}

void stm32_set_flash_latency(int latency)
{
    uint32_t code;
    switch (latency)
    {
    case 0:
        code = FLASH_ACR_LATENCY_0WS;
        break;
    case 1:
        code = FLASH_ACR_LATENCY_1WS;
        break;
    case 2:
        code = FLASH_ACR_LATENCY_2WS;
        break;
    case 3:
        code = FLASH_ACR_LATENCY_3WS;
        break;
    case 4:
        code = FLASH_ACR_LATENCY_4WS;
        break;
    case 5:
        code = FLASH_ACR_LATENCY_5WS;
        break;
    case 6:
        code = FLASH_ACR_LATENCY_6WS;
        break;
    case 7:
        code = FLASH_ACR_LATENCY_7WS;
        break;

    default:
        BUG();
    }

    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | code;
}

#ifdef FLASH_ACR_ICEN
void stm32_enable_icache()
{
    FLASH->ACR |= FLASH_ACR_ICEN;
}
#endif

#ifdef FLASH_ACR_DCEN
void stm32_enable_dcache()
{
    FLASH->ACR |= FLASH_ACR_DCEN;
}
#endif