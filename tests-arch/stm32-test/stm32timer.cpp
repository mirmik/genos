#include <igris/time/systime.h>
#include <nos/check.h>
#include <onboardtest/test.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/stm32_nvic.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_timer.h>

static int a = 0;

void ovf_handler(void *arg)
{
    a = 1;
    stm32_timer_drop_ovf_flag(TIM4);
}

void init_timer4(void)
{
    nvic_enable_irq(TIM4_IRQn);
    stm32_rcc_enable_timer(TIM4);
    stm32_timer_enable(TIM4, false);
    stm32_timer_ovf_irq_enable(TIM4, false);
    stm32_timer_set_one_pulse_mode(TIM4, false);
    stm32_timer_set_center_aligned_mode(TIM4, false);
    stm32_timer_set_direction(TIM4, TimerDirectionUp);
    stm32_timer_set_counter(TIM4, 0);
    stm32_timer_set_period(TIM4, 100);
    stm32_timer_set_prescaler(TIM4, 100);
}

TEST_CASE("stm32.timer test")
{
    init_timer4();
    igris::delay(10);
    NOS_CHECK_EQ(stm32_timer_counter(TIM4), 0);

    stm32_timer_enable(TIM4, true);
    igris::delay(10);

    NOS_CHECK_NEQ(stm32_timer_counter(TIM4), 0);
}

TEST_CASE("stm32.timer onepulse mode")
{
    init_timer4();
    stm32_timer_set_one_pulse_mode(TIM4, true);
    stm32_timer_enable(TIM4, true);
    igris::delay(10);
    NOS_CHECK_EQ(stm32_timer_counter(TIM4), 0);
}

TEST_CASE("stm32.timer counter direction up")
{
    init_timer4();
    stm32_timer_set_direction(TIM4, TimerDirectionUp);
    stm32_timer_enable(TIM4, true);
    igris::delay(10);
    NOS_CHECK_NEQ(stm32_timer_counter(TIM4), 0);
}

TEST_CASE("stm32.timer counter direction down")
{
    init_timer4();
    stm32_timer_set_direction(TIM4, TimerDirectionDown);
    stm32_timer_enable(TIM4, true);
    igris::delay(10);
    NOS_CHECK_NEQ(stm32_timer_counter(TIM4), 0);
}

TEST_CASE("stm32.timer ovf interrupt")
{
    init_timer4();
    irqtable_set_handler(TIM4_IRQn, ovf_handler, TIM4);
    stm32_timer_ovf_irq_enable(TIM4, true);
    stm32_timer_enable(TIM4, true);
    igris::delay(10);
    NOS_CHECK_EQ(a, 1);
}