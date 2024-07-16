#include <feedback.h>
#include <igris/sync/syslock.h>
#include <nos/fprint.h>
#include <ralgo/linalg/vecops.h>
#include <string>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/stm32_gpio.h>
#include <zillot/stm32/stm32_nvic.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/timer.h>

std::string encoder_state_as_string()
{
    return nos::format("TIM3 state: {}\n\rTIM2 state: {}\n\r"
                       "TIM1 state: {}\n\rTIM4 state: {}\n\r",
                       stm32_timer_counter(TIM3),
                       stm32_timer_counter(TIM2),
                       stm32_timer_counter(TIM1),
                       stm32_timer_counter(TIM4));
}

std::array<int16_t, TOTAL_CTRL_AXES> last_readed_feedback_position;
std::array<int64_t, TOTAL_CTRL_AXES> accumulator;

std::array<int64_t, TOTAL_CTRL_AXES> feedback_position_as_array()
{
    std::array<int16_t, TOTAL_CTRL_AXES> arr;
    std::array<int64_t, TOTAL_CTRL_AXES> accumulator2;
    system_lock();
    arr[0] = stm32_timer_counter(TIM2);
    arr[1] = stm32_timer_counter(TIM3);
    arr[2] = stm32_timer_counter(TIM1);
    // arr[3] = stm32_timer_counter(LPTIM1); // unused
    arr[4] = stm32_timer_counter(TIM4);
    system_unlock();

    std::array<int16_t, TOTAL_CTRL_AXES> diff;
    ralgo::vecops::sub_vv_to(diff, arr, last_readed_feedback_position); // Здесь
    ralgo::vecops::add_vv_to(accumulator, diff, accumulator);
    for (int i = 0; i < TOTAL_CTRL_AXES; i++)
    {
        last_readed_feedback_position[i] = arr[i];
        accumulator2[i] = accumulator[i];
    }
    return accumulator2;
}

void feedback_position_set(size_t ax, int64_t pos)
{
    system_lock();
    accumulator[ax] = pos;
    system_unlock();
}

void setup_port(zillot::stm32::pin pin, uint8_t af)
{
    stm32_gpio_setup(pin.regs, pin.mask, GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(pin.regs, pin.mask, af);
}

void fb_configure_common(GPIO_TypeDef *agpio,
                         GPIO_TypeDef *bgpio,
                         int amask,
                         int bmask,
                         TIM_TypeDef *tim,
                         uint8_t af)
{
    stm32_rcc_enable_gpio(agpio);
    stm32_rcc_enable_gpio(agpio);
    stm32_rcc_enable_timer(tim);
    setup_port({agpio, amask}, af);
    setup_port({bgpio, bmask}, af);
    stm32_timer_encoder_interface_config(tim,
                                         TimerEncoderMode_TI12,
                                         TimerICPolarityFalling,
                                         TimerICPolarityRising);
    stm32_timer_set_counter(tim, 0);
    stm32_timer_enable(tim, true);
    // nvic_set_priority(timirq, 0);
    // nvic_enable_irq(timirq);
}

void fb_timers_configure()
{
    fb_configure_common(
        GPIOA, GPIOB, (1 << 5), (1 << 3), TIM2, GPIO_AF_TIM2); // ch1
    fb_configure_common(
        GPIOB, GPIOB, (1 << 4), (1 << 5), TIM3, GPIO_AF_TIM3); // ch2
    fb_configure_common(
        GPIOA, GPIOA, (1 << 8), (1 << 9), TIM1, GPIO_AF_TIM1); // ch3
    fb_configure_common(
        GPIOD, GPIOD, (1 << 12), (1 << 13), TIM4, GPIO_AF_TIM4); // ch5
}

/*// Декодер квадратурного импульсного сигнала
void fb3_tim1_configure()
{
    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_timer(TIM1);
    stm32_gpio_setup(GPIOA, (1 << 8), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOA, (1 << 8), GPIO_AF_TIM1);
    stm32_gpio_setup(GPIOA, (1 << 9), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOB, (1 << 9), GPIO_AF_TIM1);
    stm32_timer_encoder_interface_config(TIM1,
                                         TimerEncoderMode_TI12,
                                         TimerICPolarityFalling,
                                         TimerICPolarityRising);
    stm32_timer_set_counter(TIM1, 0);
    stm32_timer_enable(TIM1, true);
    // nvic_set_priority(TIM1_IRQn, 0);
    // nvic_enable_irq(TIM1_IRQn);
}

// Декодер квадратурного импульсного сигнала
void fb5_tim4_configure()
{
    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_timer(TIM1);
    stm32_gpio_setup(GPIOA, (1 << 12), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOA, (1 << 12), GPIO_AF_TIM1);
    stm32_gpio_setup(GPIOA, (1 << 13), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOB, (1 << 13), GPIO_AF_TIM1);
    stm32_timer_encoder_interface_config(TIM1,
                                         TimerEncoderMode_TI12,
                                         TimerICPolarityFalling,
                                         TimerICPolarityRising);
    stm32_timer_set_counter(TIM1, 0);
    stm32_timer_enable(TIM1, true);
    // nvic_set_priority(TIM1_IRQn, 0);
    // nvic_enable_irq(TIM1_IRQn);
}*/

void axes_feedback_init()
{
    fb_timers_configure();
};