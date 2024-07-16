#include "curboard.h"

#include <zillot/stm32/stm32_clockbus.h>
#include <zillot/stm32/stm32_diag.h>
#include <zillot/stm32/stm32_gpio.h>
#include <zillot/stm32/stm32_pll.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_systick.h>
#include <zillot/stm32/stm32_usart.h>

#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>
#include <igris/util/cpu_delay.h>
#include <zillot/stm32/stm32_gpio.h>

#include <spl/stm32f4xx_gpio.h>

zillot::stm32::usart usart2(USART2, USART2_IRQn);
zillot::stm32::usart usart6(USART6, USART6_IRQn);

zillot::stm32::pin board_led(GPIOD, 1 << 14);
zillot::stm32::pin board_led2(GPIOD, 1 << 15);

zillot::stm32::pin extpin[4] = {
    {GPIOB, 1 << 12}, {GPIOB, 1 << 13}, {GPIOB, 1 << 14}, {GPIOB, 1 << 15}};

extern "C" void arch_init();
void board_init()
{
    arch_init();

    struct stm32_pll_settings pll_settings = {
        .Mkoeff = 8,
        .Nkoeff = 336,
        .Pkoeff = 2,
        .Qkoeff = 7}; // 84 000 000 Гц //168 000 000

    stm32_init_pll_clocking(&pll_settings);
    stm32_systick_config(168000);
    systime_set_frequency(1000);

    stm32_clockbus_freq[CLOCKBUS_PLL] = 168000000;
    stm32_clockbus_set_hpre_divider(1);
    stm32_clockbus_set_ppre1_divider(4);
    stm32_clockbus_set_ppre2_divider(2);

    stm32_clockbus_freq[CLOCKBUS_APB1] =
        stm32_clockbus_freq[CLOCKBUS_PLL] / stm32_clockbus_get_ppre1_divider();
    stm32_clockbus_freq[CLOCKBUS_APB2] =
        stm32_clockbus_freq[CLOCKBUS_PLL] / stm32_clockbus_get_ppre2_divider();

    stm32_rcc_enable_usart(USART2);
    stm32_rcc_enable_usart(USART6);
    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_gpio(GPIOC);
    stm32_rcc_enable_gpio(GPIOD);

    cpu_delay(100);

    stm32_gpio_setup(GPIOA, (1 << 2 | 1 << 3), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_maxspeed(GPIOA, (1 << 2 | 1 << 3), STM32_GPIO_SPEED_LEVEL_0);
    stm32_gpio_set_alternate(GPIOA, (1 << 2 | 1 << 3), GPIO_AF_USART2);

    stm32_usart_setup(USART2, 115200, 'n', 8, 1);
    stm32_diag_init(USART2);

    // board::sysled0.settings(GPIO_MODE_OUTPUT);
    // board::sysled1.settings(GPIO_MODE_OUTPUT);

    board_led.set(0);
    board_led2.set(0);
    board_led.setup(GPIO_MODE_OUTPUT);
    board_led2.setup(GPIO_MODE_OUTPUT);
}