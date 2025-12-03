#include <asm/irq.h>
#include <fcntl.h>
#include <genos/autom_schedee.h>
#include <genos/coop_schedee.h>
#include <genos/drivers/tty_linedisc_line.h>
#include <genos/drivers/zillot_tty_driver.h>
#include <genos/ktimer.h>
#include <genos/schedee.h>
#include <igris/sync/syslock.h>
#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>
#include <igris/util/cpu_delay.h>
#include <nos/io/generic_ostream.h>
#include <nos/print.h>
#include <unistd.h>
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

zillot::stm32::usart usart8{UART8, UART8_IRQn};

genos::zillot_tty_driver tty_driver(&usart8, 48);
genos::tty_linedisc_line tty_line;
genos::tty tty("ttyS0", &tty_driver, &tty_line);

void enable_pll_clocking()
{
    stm32_disable_pll();

    stm32h7_set_pll1_source_hse();
    stm32h7_set_pll1_coefficients(16, 72, 4);

    auto sourse_freq = stm32_clockbus_freq[CLOCKBUS_HSE];
    int64_t sysfreq = sourse_freq / 16 * 72 / 4;

    stm32_enable_pll();
    stm32_system_clock_mux_pll(0);
    stm32_clockbus_freq[CLOCKBUS_PLL] = sysfreq;
}

void enable_hse_clocking()
{
    int systick_freq = 1000;
    int64_t sysfreq = stm32_clockbus_freq[CLOCKBUS_HSE];
    stm32_systick_config(sysfreq / systick_freq);
    systime_set_frequency(systick_freq);
    irqtable_set_handler(
        SysTick_IRQn, (void (*)(void *))jiffies_increment, NULL);
}

nos::generic_ostream stdostream(+[](const void *data, size_t size) -> int
                                {
                                    debug_write((const char *)data, size);
                                    return 1;
                                });

int mainthread(void *arg)
{
    debug_print("coop schedee started\r\n");

    int a = open("/dev/ttyS0", O_RDONLY);
    int b = open("/dev/ttyS0", O_WRONLY);

    char buf[32];
    while (1)
    {
        int sz = read(STDIN_FILENO, buf, 32);
        nos::println("readed: ", sz);
        //    nos::write(buf, sz);
    }
    return 0;
}

int main()
{
    genos::schedee_manager_init();
    genos::ktimer_manager_init();

    nos::set_default_ostream(&stdostream);
    stm32_clockbus_freq[CLOCKBUS_HSI] = 64000000;
    stm32_clockbus_freq[CLOCKBUS_HSE] = 25000000;

    irqs_disable();
    irqtable_init();

    // enable clocking from hse
    stm32_enable_hse();
    stm32_system_clock_mux_hse();

    stm32_clockbus_set_d1cpre_divider(1);
    stm32_clockbus_set_hpre_divider(1);
    stm32_clockbus_set_d1ppre_divider(1);
    stm32_clockbus_set_d2ppre1_divider(1);
    stm32_clockbus_set_d2ppre2_divider(1);
    stm32_clockbus_set_d3ppre_divider(1);

    enable_pll_clocking();
    clockbus_reeval_clocks();

    int systick_freq = 1000;
    stm32_systick_config(stm32_clockbus_freq[CLOCKBUS_SYSTICK] / systick_freq);
    systime_set_frequency(systick_freq);
    irqtable_set_handler(
        SysTick_IRQn, (void (*)(void *))jiffies_increment, NULL);

    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_gpio(GPIOC);
    stm32_rcc_enable_gpio(GPIOE);
    stm32_rcc_enable_usart(UART8);

    zillot::stm32::pin(GPIOE, 1 << 0).setup(GPIO_MODE_ALTERNATE);
    zillot::stm32::pin(GPIOE, 1 << 1).setup(GPIO_MODE_ALTERNATE);
    zillot::stm32::pin(GPIOE, 1 << 0).setup_alternate(8);
    zillot::stm32::pin(GPIOE, 1 << 1).setup_alternate(8);

    stm32_usart_setup(UART8, 115200, 'n', 8, 1);
    stm32_usart_enable(UART8, true);
    stm32_diag_init(UART8);

    green_led.setup(GPIO_MODE_OUTPUT);
    red_led.setup(GPIO_MODE_OUTPUT);

    green_led.set(0);
    red_led.set(1);

    irqs_enable();

    tty_driver.setup(115200, 'n', 8, 1);
    tty_driver.begin();

    genos::create_coop_schedee(mainthread, NULL, NULL, 1024)->start();

    while (1)
    {
        __schedule__();
    }
}

void __schedule__()
{
    genos::ktimer_manager_step();
    genos::schedee_manager_step();
}

extern "C" void emergency_halt();
void emergency_halt()
{
    irqs_disable();
    while (1)
    {
    }
}