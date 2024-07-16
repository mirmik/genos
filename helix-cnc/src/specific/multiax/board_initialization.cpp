#include <feedback.h>
#include <genos/chardev.h>
#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>
#include <igris/util/bits.h>
#include <nos/io/generic_ostream.h>
#include <nos/print.h>
#include <zillot/armv7e-m/arm_nvic.h>
#include <zillot/common/uartring.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/stm32_diag.h>
#include <zillot/stm32/stm32_gpio.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_systick.h>
#include <zillot/stm32/stm32_usart.h>
#include <zillot/stm32/stm32h7_pll.h>
#include <zillot/stm32/usart.h>

zillot::stm32::pin green_led{GPIOB, 1 << 6};
zillot::stm32::pin red_led{GPIOB, 1 << 7};

zillot::stm32::usart usart_console{UART8, UART8_IRQn};
zillot::stm32::usart usart_rs232{USART10, USART10_IRQn};
// zillot::stm32::usart usart_rs232{UART8, UART8_IRQn};
// zillot::stm32::usart usart_console{USART10, USART10_IRQn};

void toggle_system_led()
{
    green_led.toggle();
}

void user_led_set(uint8_t state)
{
    red_led.set(state);
}

void board_specific_start() {}

void enable_pll_clocking()
{
    stm32_disable_pll();

    constexpr int M = 15;
    constexpr int N = 15 * 15;
    constexpr int P = 1; // 425

    // constexpr int M = 15;
    // constexpr int N = 252;

    //    constexpr int M = 12;
    //    constexpr int N = 204; // 425 MHz

    //    constexpr int M = 10;
    //    constexpr int N = 170;
    //    constexpr int P = 1; // 425

    // constexpr int M = 8;
    // constexpr int N = 272;
    // constexpr int P = 2; // 425

    // constexpr int M = 12;
    // constexpr int N = 204;
    // constexpr int P = 1; // 425

    stm32h7_set_pll1_source_hse();
    stm32h7_set_pll1_coefficients(M, N, P);

    auto sourse_freq = stm32_clockbus_freq[CLOCKBUS_HSE];
    int64_t sysfreq = sourse_freq / M * N / P;

    stm32_enable_pll();
    stm32_system_clock_mux_pll(0);
    stm32_clockbus_freq[CLOCKBUS_PLL] = sysfreq;
}

nos::generic_ostream debug_debug_console_ostream(+[](const void *data,
                                                     size_t size) -> int {
    debug_write((const char *)data, size);
    return 1;
});

void __schedule__();
void board_initialization()
{
    stm32_clockbus_freq[CLOCKBUS_HSI] = 64000000;
    stm32_clockbus_freq[CLOCKBUS_HSE] = 25000000;

    irqs_disable();
    irqtable_init();

    // enable clocking from hse
    stm32_enable_hse();
    stm32_system_clock_mux_hse();

    stm32_clockbus_set_d1cpre_divider(1);
    stm32_clockbus_set_hpre_divider(4);
    stm32_clockbus_set_d1ppre_divider(1);
    stm32_clockbus_set_d2ppre1_divider(1);
    stm32_clockbus_set_d2ppre2_divider(1);
    stm32_clockbus_set_d3ppre_divider(1);

    stm32_set_flash_latency(4);

    enable_pll_clocking();
    clockbus_reeval_clocks();

    int systick_freq = 1000;
    stm32_systick_config(stm32_clockbus_freq[CLOCKBUS_SYSTICK] / systick_freq);
    systime_set_frequency(systick_freq);
    irqtable_set_handler(
        SysTick_IRQn, (void (*)(void *))jiffies_increment, NULL);

    nvic_set_priority(SysTick_IRQn, 0);

    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_gpio(GPIOC);
    stm32_rcc_enable_gpio(GPIOD);
    stm32_rcc_enable_gpio(GPIOE);
    stm32_rcc_enable_gpio(GPIOF);
    stm32_rcc_enable_gpio(GPIOG);
    stm32_rcc_enable_usart(UART8);
    stm32_rcc_enable_usart(USART10);

    zillot::stm32::pin(GPIOE, 1 << 0).setup(GPIO_MODE_ALTERNATE);
    zillot::stm32::pin(GPIOE, 1 << 1).setup(GPIO_MODE_ALTERNATE);
    zillot::stm32::pin(GPIOE, 1 << 0).setup_alternate(8);
    zillot::stm32::pin(GPIOE, 1 << 1).setup_alternate(8);

    zillot::stm32::pin(GPIOE, 1 << 2).setup(GPIO_MODE_ALTERNATE);
    zillot::stm32::pin(GPIOE, 1 << 3).setup(GPIO_MODE_ALTERNATE);
    zillot::stm32::pin(GPIOE, 1 << 2).setup_alternate(4);
    zillot::stm32::pin(GPIOE, 1 << 3).setup_alternate(11);

    stm32_usart_setup(UART8, 115200, 'n', 8, 1);
    stm32_usart_enable(UART8, true);
    stm32_diag_init(UART8);

    stm32_usart_setup(USART10, 115200, 'n', 8, 1);
    stm32_usart_enable(USART10, true);

    nvic_set_priority(UART8_IRQn, 1);
    nvic_set_priority(USART10_IRQn, 1);

    green_led.setup(GPIO_MODE_OUTPUT);
    red_led.setup(GPIO_MODE_OUTPUT);

    green_led.set(0);
    red_led.set(1);

    irqs_enable();
}

void init_control_pins();
void init_debug_console();
void configure()
{
    init_control_pins();
    axes_feedback_init();
}
