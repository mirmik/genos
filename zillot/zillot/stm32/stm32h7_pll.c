#include <zillot/stm32/stm32h7_pll.h>
#include <zillot/stm32/stm32_rcc.h>

/*unsigned stm32h7_init_pll_clocking(struct stm32h7_pll_settings* settings) 
{
    volatile int i;
    // Включаем масштабирование регулятора напряжения.
    // Необходимо для высоких частот ???
    //	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    //	RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;
    //	bits_assign(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_0);

    /// Настраиваем задержки FLASH для работы при повышенной частоте.
    // FLASH->ACR = //FLASH_ACR_ICEN; //| FLASH_ACR_DCEN |
    // FLASH_ACR_LATENCY_5WS; FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_DCEN |
    // FLASH_ACR_LATENCY_4WS; i = 1000; while (--i);

#if 0
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN; // Power interface clock enable
    i = 1000;
    while (--i)
        ;
    while ((RCC->APB1ENR1 & RCC_APB1ENR1_PWREN) != RCC_APB1ENR1_PWREN)
    {
    };
#endif

#if 0
    bits_assign(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_0);
    i = 1000;
    while (--i)
        ;
    while ((PWR->CR1 & PWR_CR1_VOS) != PWR_CR1_VOS_0)
    {
    }; // Waits until scaling is ready

    bits_assign(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);
    bits_assign(FLASH->ACR, FLASH_ACR_PRFTEN, FLASH_ACR_PRFTEN);
    bits_assign(FLASH->ACR, FLASH_ACR_ICEN, FLASH_ACR_ICEN);
    bits_assign(FLASH->ACR, FLASH_ACR_DCEN, FLASH_ACR_DCEN);

#endif
    // Запуск HSE и ожидание включения
    stm32_enable_hse();

#if 0
    // Конфигурируем PLL, выбирая в качестве источника внешний кварц.
    RCC->PLLCFGR =
        (RCC_PLLCFGR_PLLSRC_HSE |
         (RCC_PLLCFGR_PLLM & ((s->Mkoeff - 1) << RCC_PLLCFGR_PLLM_Pos)) |
         (RCC_PLLCFGR_PLLN &
          (s->Nkoeff << RCC_PLLCFGR_PLLN_Pos)) // 8 >= PLLN >= 127
         | (RCC_PLLCFGR_PLLR & (0b00 << RCC_PLLCFGR_PLLR_Pos)) |
         (RCC_PLLCFGR_PLLP & (0b00 << RCC_PLLCFGR_PLLP_Pos)) |
         (RCC_PLLCFGR_PLLQ & (0b00 << RCC_PLLCFGR_PLLQ_Pos)));
#endif

#if 0
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    // Активируем PLL, ждём, пока он запустится.
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY))
        ;

    // Тактируем шину от PLL, ждём подтверждения.
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= (RCC_CFGR_SW_PLL);
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL)
        ;

#endif
    return 0;    
}
*/