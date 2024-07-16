#include <asm/irq.h>
#include <igris/time/jiffies-systime.h>
#include <igris/time/systime.h>
#include <igris/util/bug.h>
#include <igris/util/cpu_delay.h>
#include <math.h>
#include <music.h>
#include <stm32xxxx.h>
#include <vector>
#include <zillot/armv7e-m/arm_nvic.h>
#include <zillot/board_pinout/nucleof446re.h>
#include <zillot/common/uartring.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/stm32_clockbus.h>
#include <zillot/stm32/stm32_diag.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_systick.h>
#include <zillot/stm32/stm32_timer.h>
#include <zillot/stm32/stm32_usart.h>
#include <zillot/stm32/usart.h>

// NUCLEO-F446RE
// Buzzer on D3 pin

#define HSI_FREQ 16000000

// stm32f401re led
zillot::stm32::pin board_led(GPIOA, 1 << 5);
zillot::stm32::usart board_usart(USART2, USART2_IRQn);
ZILLOT_UARTRING(serial2, board_usart, 128, 128);

void _write(const char *str, size_t len)
{
    serial2.write(str, len);
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

void set_pwm_freq(double sound_freq)
{
    double cpufreq = HSI_FREQ;
    double div = cpufreq / sound_freq;
    int prescaler = div / 65536 + 1;
    int freq = div / prescaler;
    double real_freq = cpufreq / (prescaler * freq);

    stm32_timer_set_prescaler(TIM2, prescaler);
    stm32_timer_set_period(TIM2, freq);
    stm32_timer_channel_set_compare(TIM2, 2, freq / 2);
    stm32_timer_set_counter(TIM2, 0);
}

int main()
{
    irqtable_init();
    conf_clock();
    init_systick_hsi();

    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_usart(USART2);
    stm32_rcc_enable_timer(TIM2);

    conf_usart2();
    board_led.setup(GPIO_MODE_OUTPUT);
    board_usart.setup(115200, 'n', 8, 1);
    stm32_diag_init(USART2);

    serial2.begin();

    // timer2, ch2
    zillot::nucleo_f446::PINOUT[3].setup_alternate(GPIO_AF_TIM2);

    stm32_timer_enable(TIM2, 1);
    stm32_timer_channel_enable_output(TIM2, 2);
    stm32_timer_channel_enable_pwm(TIM2, 2);
    stm32_timer_set_counter(TIM2, 0);

    std::vector<MusicSign> amazing_grace = parse_flipper_music(
        140,
        8,
        5,
        "4C, 2F, A, F, 2A, 4G, 2F, 4D, 2C, 4C, 2F, A, F, 2A, 4G, 1C6, 4A, "
        "4C6., A, C6, A, 2F, 4C, 4D., F, F, D, 2C, 4C, 2F, A, F, 2A, 4G, 1F");

    std::vector<MusicSign> hedwig_theme = parse_flipper_music(
        260,
        4,
        5,
        "B4, E., 8G, F#, 2E, B, 2A., 2F#., E., 8G, F#, 2D#, F, 2B4., 2P, B4, "
        "E., 8G, F#, 2E, B, 2D6, C#6, 2C6, G#, C6., 8B, A#, 2A#4, G, 2E5., 2P, "
        "G, 2B, G, 2B, G, 2C6, B, 2A#, F#, G., 8B, A#, 2A#4, B4, 2B., 2P, G, "
        "2B, G, 2B, G, 2D6, C#6, 2C6, G#, C6., 8B, A#, 2A#4, G, 2E., 2P");

    std::vector<MusicSign> marble_machine = parse_flipper_music(
        130,
        8,
        5,
        "E6, P, E, B, 4P, E, A, G, A, E, B, P, G, A, D6, 4P, D, B, 4P, D, A, "
        "G, A, D, F#, P, G, A, D6, 4P, F#, B, 4P, F#, D6, C6, B, F#, A, P, G, "
        "F#, E, P, C, E, B, B4, C, D, D6, C6, B, F#, A, P, G, A, E6, 4P, E, B, "
        "4P, E, A, G, A, E, B, P, G, A, D6, 4P, D, B, 4P, D, A, G, A, D, F#, "
        "P, G, A, D6, 4P, F#, B, 4P, F#, D6, C6, B, F#, A, P, G, F#, E, P, C, "
        "E, B, B4, C, D, D6, C6, B, F#, A, P, G, A, E6");

    irqs_enable();

    std::vector<std::vector<MusicSign>> musics = {
        amazing_grace, hedwig_theme, marble_machine};
    while (1)
    {
        for (auto music : musics)
        {
            for (auto s : music)
            {
                dpr(s.text.c_str());
                dpr(" ");
                dprln(s.octave_no);

                stm32_timer_enable(TIM2, 1);
                stm32_timer_set_counter(TIM2, 0);
                if (s.freq != 0)
                    set_pwm_freq(s.freq);
                else
                    stm32_timer_enable(TIM2, 0);
                igris::delay(s.duration);
            }
            stm32_timer_enable(TIM2, 0);
            igris::delay(1000);
        }
    }
    stm32_timer_enable(TIM2, 0);

    while (1)
    {
        // dprln(stm32_timer_counter(TIM2));
        board_led.toggle();
        igris::delay(100);
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