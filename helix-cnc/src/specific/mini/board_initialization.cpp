#include <asm/irq.h>
#include <curboard.h>
#include <igris/time/jiffies-systime.h>
#include <igris/util/cpu_delay.h>
#include <stm32xxxx.h>
#include <zillot/armv7e-m/arm_nvic.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/stm32_clockbus.h>
#include <zillot/stm32/stm32_diag.h>
#include <zillot/stm32/stm32_gpio.h>
#include <zillot/stm32/stm32_pll.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_systick.h>
#include <zillot/stm32/stm32_usart.h>
#include <zillot/stm32/usart.h>

zillot::stm32::usart usart_console(USART2, USART2_IRQn);
zillot::stm32::usart usart_rs232(USART6, USART6_IRQn);
zillot::stm32::pin board_led(GPIOD, 1 << 14);
zillot::stm32::pin board_led2(GPIOD, 1 << 15);

zillot::stm32::pin extpin[4] = {
    {GPIOB, 1 << 12}, {GPIOB, 1 << 13}, {GPIOB, 1 << 14}, {GPIOB, 1 << 15}};

namespace board
{
    zillot::stm32::usart sysuart(USART2, USART2_IRQn);
    zillot::stm32::pin sysled(GPIOD, 1 << 14);
}

void toggle_system_led()
{
    board_led.toggle();
}

void board_initialization()
{
    board_init(0);
    usart_console.setup(115200, 'n', 8, 1);
    usart_rs232.setup(115200, 'n', 8, 1);

    nvic_set_priority(SysTick_IRQn, 1);
    nvic_set_priority(TIM4_IRQn, 0); // bottom control layer
    nvic_set_priority(USART2_IRQn, 2);
    nvic_set_priority(USART6_IRQn, 2);
}

void user_led_set(uint8_t state)
{
    board_led2.set(state);
}

extern "C" void arch_init();

void usart2_configure()
{
    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_usart(USART2);
    cpu_delay(100);
    stm32_gpio_setup(GPIOA, (1 << 2 | 1 << 3), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_maxspeed(GPIOA, (1 << 2 | 1 << 3), STM32_GPIO_SPEED_LEVEL_0);
    stm32_gpio_set_alternate(GPIOA, (1 << 2 | 1 << 3), GPIO_AF_USART2);
    nvic_enable_irq(USART2_IRQn);
    stm32_usart_setup(USART2, 115200, 'n', 8, 1);
    stm32_diag_init(USART2);
}

void usart6_configure()
{
    stm32_rcc_enable_gpio(GPIOC);
    stm32_rcc_enable_usart(USART6);
    cpu_delay(100);
    stm32_gpio_setup(GPIOC, (1 << 6 | 1 << 7), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_maxspeed(GPIOC, (1 << 6 | 1 << 7), STM32_GPIO_SPEED_LEVEL_0);
    stm32_gpio_set_alternate(GPIOC, (1 << 6 | 1 << 7), GPIO_AF_USART6);
    nvic_enable_irq(USART6_IRQn);
    stm32_usart_setup(USART6, 115200, 'n', 8, 1);
}

#define HSI_FREQ 16000000
void stm32_sysinit();

void system_tick();
static void init_systick_hsi()
{
    stm32_systick_config(HSI_FREQ / 1000);
    irqtable_set_handler(
        SysTick_IRQn, (void (*)(void *))jiffies_increment, NULL);
}

extern "C" void arch_init();
void arch_init()
{
    irqs_disable();

    stm32_clockbus_freq[CLOCKBUS_HSI] = HSI_FREQ;
    irqtable_init();
    init_systick_hsi();
}

void __exidx_start()
{
    BUG();
}

void __exidx_end()
{
    BUG();
}

void board_init(int mode)
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

    // stm32_pll_setup(8, 336, 2, 7);
    // stm32_systick_config(168000);

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

    usart2_configure();
    usart6_configure();

    board_led.setup(GPIO_MODE_OUTPUT);
    board_led2.setup(GPIO_MODE_OUTPUT);

#ifdef LPUART1_IRQn
    nvic_set_priority(LPUART1_IRQn, 2);
#endif
}