#include <board_defs.h>
#include <genos/autom_schedee.h>
#include <genos/execution_monitor.h>
#include <main.h>
#include <ralgo/cnc/revolver.h>
#include <ralgo/cnc/shift.h>
#include <ralgo/robo/stepper_controller.h>
#include <zillot/armv7e-m/arm_nvic.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/stm32_rcc.h>
#include <zillot/stm32/stm32_timer.h>

int64_t BOTTOM_FREQUENCY_MEASURE_COUNTER = 0;

genos::execution_monitor bottom_level_execmon("blevel");

void control_bottom_level();

int32_t serve_handler_count = 0;
void serve_handler(void *arg)
{
    (void)arg;
    serve_handler_count++;
    stm32_timer_drop_ovf_flag(SERVE_TIM);
    control_bottom_level();
}

int32_t get_serve_handler_count()
{
    return serve_handler_count;
}

void disable_bottom_level()
{
    stm32_timer_ovf_irq_enable(SERVE_TIM, false);
    stm32_timer_enable(SERVE_TIM, false);
}

void enable_bottom_level()
{
    stm32_timer_enable(SERVE_TIM, true);
    stm32_timer_ovf_irq_enable(SERVE_TIM, true);
}

void init_bottom_level()
{
    double prescaller = BOTTOM_LEVEL_PRESCALLER;
    double period = BOTTOM_LEVEL_PERIOD;

    stm32_rcc_enable_timer(SERVE_TIM);
    stm32_timer_set_prescaler(SERVE_TIM, prescaller);
    stm32_timer_set_period(SERVE_TIM, period);

    irqtable_set_handler(SERVE_TIM_IRQn, serve_handler, NULL);
    nvic_enable_irq(SERVE_TIM_IRQn);
    nvic_set_priority(SERVE_TIM_IRQn, 1);

    enable_bottom_level();

    double freq = BOTTOM_LEVEL_TIMER_FREQ /
                  (BOTTOM_LEVEL_PRESCALLER * BOTTOM_LEVEL_PERIOD);
    nos::println("Bottom level initialization ... OK");
    nos::println("Bottom freq: ", freq);
}

static int64_t start_serve_handler_count;
void frequency_test_start()
{
    system_lock();
    start_serve_handler_count = serve_handler_count;
    system_unlock();
}

void frequency_test_stop()
{
    system_lock();
    int64_t stop_serve_handler_count = serve_handler_count;
    system_unlock();
    BOTTOM_FREQUENCY_MEASURE_COUNTER =
        stop_serve_handler_count - start_serve_handler_count;
}

double fast_cycle_frequence()
{
    double prescaller = BOTTOM_LEVEL_PRESCALLER;
    double period = BOTTOM_LEVEL_PERIOD;
    double freq = BOTTOM_LEVEL_TIMER_FREQ;
    double tiks_per_second = freq / prescaller / period;
    return tiks_per_second;
}
