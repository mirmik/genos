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
    return nos::format("TIM5 state: {}\n\rTIM2 state: {}\n\r",
                       stm32_timer_counter(TIM5),
                       stm32_timer_counter(TIM2));
}

std::array<int32_t, TOTAL_CTRL_AXES> last_readed_feedback_position;
std::array<int64_t, TOTAL_CTRL_AXES> accumulator;

std::array<int64_t, TOTAL_CTRL_AXES> feedback_position_as_array()
{
    std::array<int32_t, TOTAL_CTRL_AXES> arr;
    std::array<int64_t, TOTAL_CTRL_AXES> accumulator2;
    system_lock();
    arr[0] = stm32_timer_counter(TIM2);
    arr[1] = stm32_timer_counter(TIM5);
    system_unlock();

    std::array<int64_t, TOTAL_CTRL_AXES> diff;
    ralgo::vecops::sub_vv_to(diff, arr, last_readed_feedback_position);
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

void tim5_configure()
{
    stm32_rcc_enable_timer(TIM5);
    stm32_rcc_enable_gpio(GPIOA);
    stm32_gpio_setup(GPIOA, (1 << 0) | (1 << 1), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOA, (1 << 0) | (1 << 1), GPIO_AF_TIM5);

    stm32_timer_encoder_interface_config(TIM5,
                                         TimerEncoderMode_TI12,
                                         TimerICPolarityFalling,
                                         TimerICPolarityRising);
    stm32_timer_set_counter(TIM5, 0);
    stm32_timer_enable(TIM5, true);

    nvic_set_priority(TIM5_IRQn, 0);
    nvic_enable_irq(TIM5_IRQn);
}

// Декодер квадратурного импульсного сигнала
void tim2_configure()
{
    stm32_rcc_enable_gpio(GPIOA);
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_timer(TIM2);

    stm32_gpio_setup(GPIOA, (1 << 5), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOA, (1 << 5), GPIO_AF_TIM2);

    stm32_gpio_setup(GPIOB, (1 << 3), GPIO_MODE_ALTERNATE);
    stm32_gpio_set_alternate(GPIOB, (1 << 3), GPIO_AF_TIM2);

    stm32_timer_encoder_interface_config(TIM2,
                                         TimerEncoderMode_TI12,
                                         TimerICPolarityFalling,
                                         TimerICPolarityRising);
    stm32_timer_set_counter(TIM2, 0);
    stm32_timer_enable(TIM2, true);

    nvic_set_priority(TIM2_IRQn, 0);
    nvic_enable_irq(TIM2_IRQn);
}

void tim2_compare_enable()
{
    // TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    // TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
}

void tim2_compare_disable()
{
    // TIM_ITConfig(TIM2, TIM_IT_CC3, DISABLE);
}

void tim5_compare_enable()
{
    /*    TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
        TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);*/
}

void tim5_compare_disable()
{
    // TIM_ITConfig(TIM5, TIM_IT_CC3, DISABLE);
}

void axes_feedback_init()
{
    tim2_configure();
    tim5_configure();
};