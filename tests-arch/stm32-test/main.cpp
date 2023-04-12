#include <asm/irq.h>
#include <genos/coop_schedee.h>
#include <genos/drivers/tty_linedisc_line.h>
#include <genos/drivers/zillot_tty_driver.h>
#include <genos/schedee.h>
#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>
#include <igris/util/bug.h>
#include <igris/util/cpu_delay.h>
#include <nos/check.h>
#include <onboardtest/test.h>
#include <onboardtest/testenv.h>
#include <stm32xxxx.h>
#include <zillot/armv7e-m/arm_nvic.h>
#include <zillot/common/uartring.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/stm32_clockbus.h>
#include <zillot/stm32/stm32_diag.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_systick.h>
#include <zillot/stm32/stm32_usart.h>
#include <zillot/stm32/usart.h>

#define HSI_FREQ 8000000

// stm32f401re led
zillot::stm32::pin board_led(GPIOA, 1 << 5);
zillot::stm32::usart board_usart(USART2, USART2_IRQn);
// ZILLOT_UARTRING(serial2, board_usart, 128, 128);

genos::zillot_tty_driver tty_driver(&board_usart, 48);
genos::tty_linedisc_line tty_line;
genos::tty tty("ttyS0", &tty_driver, &tty_line);

void _write(const char *str, size_t len)
{
    // serial2.write(str, len);
}

void _syncronization()
{
    // while (serial2.avail() < 1)
    // {
    //     board_led.toggle();
    //     cpu_delay(50000);
    // }

    // igris::delay(100);
    // while (serial2.avail())
    // {
    //     char c;
    //     serial2.read(&c, 1);
    // }
}

void conf_clock()
{
    stm32_clockbus_enable_hsi_mode();
    stm32_clockbus_freq[CLOCKBUS_HSI] = HSI_FREQ;
    stm32_clockbus_set_hpre_divider(1);
    stm32_clockbus_set_ppre1_divider(1);
    stm32_clockbus_set_ppre2_divider(1);
    stm32_clockbus_freq[CLOCKBUS_APB1] = HSI_FREQ;
    stm32_clockbus_freq[CLOCKBUS_APB2] = HSI_FREQ;
}

void conf_usart2()
{
    stm32_gpio_setup(GPIOA, (1 << 2 | 1 << 3), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_maxspeed(GPIOA, (1 << 2 | 1 << 3), STM32_GPIO_SPEED_LEVEL_0);
    stm32_gpio_set_alternate(GPIOA, (1 << 2 | 1 << 3), GPIO_AF_USART2);
}

static void init_systick_hsi()
{
    stm32_systick_config(HSI_FREQ / 1000);
    systime_set_frequency(1000);

    irqtable_set_handler(
        SysTick_IRQn, (void (*)(void *))jiffies_increment, NULL);
}

int main()
{
    irqtable_init();
    conf_clock();
    init_systick_hsi();

    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_usart(USART2);

    conf_usart2();
    board_led.setup(GPIO_MODE_OUTPUT);
    board_usart.setup(115200, 'n', 8, 1);

    stm32_diag_init(USART2);

    // serial2.begin();
    irqs_enable();
    tty_driver.begin();

    auto *sch = genos::create_coop_schedee(
        +[](void *priv)
        {
            while (1)
            {
                board_led.toggle();
                cpu_delay(500000);
            }
            return 0;
        },
        nullptr,
        nullptr,
        512);

    while (1)
    {
        __schedule__();
    }
}

void __schedule__()
{
    while (1)
    {
        genos::ktimer_manager_step();
        genos::schedee_manager_step();
    }
}

extern "C" int *__errno_location();
int *__errno_location()
{
    static int err;
    return &err;
}

extern "C" void emergency_halt();
void emergency_halt()
{
    while (1)
    {
        board_led.toggle();
        cpu_delay(500000);
    }
}

extern "C" void __context_displace_vector__();
void __context_displace_vector__()
{
    BUG();
}