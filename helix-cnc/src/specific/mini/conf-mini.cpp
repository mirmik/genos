#include <feedback.h>
#include <quadgen.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/stm32_clockbus.h>
#include <zillot/stm32/stm32_pll.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_systick.h>
#include <zillot/stm32/timer.h>

#define GPIO_AF_TIM10 ((uint8_t)0x03)
#define GPIO_AF_TIM5 ((uint8_t)0x02)
#define GPIO_AF_TIM2 ((uint8_t)0x01)

struct abpin
{
    zillot::stm32::pin a;
    zillot::stm32::pin b;
    zillot::stm32::pin c;
    zillot::stm32::pin d;
};

abpin mconn[2]{/// CH1
               {
                   {GPIOD, 1 << 3},
                   {GPIOB, 1 << 4},
                   {GPIOD, 1 << 5},
                   {GPIOD, 1 << 7},
               },
               {
                   /// CH2
                   {GPIOD, 1 << 2},
                   {GPIOB, 1 << 5},
                   {GPIOD, 1 << 4},
                   {GPIOD, 1 << 6},
               }

};
quadgen_stm32_4 physax[TOTAL_CTRL_AXES];

robo::quadgen *control_quadgens[TOTAL_CTRL_AXES] = {
    &physax[0],
    &physax[1],
};

void init_physaxes()
{
    for (int i = 0; i < TOTAL_CTRL_AXES; i++)
    {
        auto &ab = mconn[i];
        physax[i] = quadgen_stm32_4(ab.a, ab.b, ab.c, ab.d);
    }
}

//Таймер для генерации одиночного импульса. OneShoot.
void tim10_configure()
{
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_timer(TIM10);

    zillot::stm32::pin(GPIOB, (1 << 8)).setup(GPIO_MODE_ALTERNATE);
    zillot::stm32::pin(GPIOB, (1 << 8)).setup_alternate(GPIO_AF_TIM10);

    stm32_timer_set_prescaler(TIM10, 120);
    stm32_timer_set_period(TIM10, 6);
    stm32_timer_set_counter_mode(TIM10, STM32_TIMER_DOWNCOUNT);
    stm32_timer_set_one_pulse_mode(TIM10, 1);
    stm32_timer_set_counter(TIM10, 0);
};

void motor_pins_configure()
{
    stm32_rcc_enable_gpio(GPIOB);
    stm32_rcc_enable_gpio(GPIOD);

    zillot::stm32::pin(GPIOB, (1 << 4) | (1 << 5))
        .setup(GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);
    zillot::stm32::pin(
        GPIOD, (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))
        .setup(GPIO_MODE_OUTPUT | GPIO_MODE_OUT_PUSH_PULL);
    init_physaxes();
};

void configure()
{
    tim10_configure();
    motor_pins_configure();
    axes_feedback_init();
}
