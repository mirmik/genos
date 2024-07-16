#include "functions.h"
#include <curboard.h>

#include <assert.h>

#include <igris/dprint.h>
#include <igris/util/stub.h>
#include <zillot/common/uartring.h>

#include <genos/schedee.h>
#include <zillot/stm32/stm32_timer.h>

#define COUNT_OF_DISCRETE_OUTPUTS 4

int32_t last_scan_information = 0;
extern uint32_t trigin_divider;
int disable_trigin_mode = 0;

void install_controller(MoveController *volatile *slot, MoveController *ctr)
{
    //Активация контроллера предполагает инициализацию соотв. слота
    //и создание связанного процесса.
    *slot = ctr;
    ctr->start_schedee();
}

int32_t encoder_x()
{
    return axis_x.read_encoder();
};

int32_t encoder_y()
{
    return axis_y.read_encoder();
};

void reset_encoder_x()
{
    *axis_x.encoder_reg = 0;
};

void reset_encoder_y()
{
    *axis_y.encoder_reg = 0;
};

int32_t trigin_count()
{
    return trig_in_counter;
};

int manual_mode_counter = 0;
void manual_mode_scan_command_send()
{
    manual_mode_counter++;
    scan_command_send(manual_mode_counter);
}

//Ручная активация callback trigger для работы с nac_pulse.
extern igris::delegate<void> trigin_handler;
void manual_trig_callback(bool en)
{
    stop();

    manual_mode_counter = 0;
    eventctr_mode = false;
    disable_trigin_mode = 0;

    if (en)
        trigin_handler = manual_mode_scan_command_send;
    else
        trigin_handler = do_nothing;
};

void scan_command_send(int number)
{
    char buf[16];

    trigreturn_time = igris::millis() - trigstart_time;
    system_lock();
    serial6.write("WX", 2);

    igris_i32toa(last_scan_information, buf, 10);
    serial6.write(buf, strlen(buf));

    serial6.write("YZ", 2);

    igris_i32toa(number, buf, 10);
    serial6.write(buf, strlen(buf));

    system_unlock();
};

void external_trigger_received_send(int number, int32_t value)
{
    char buf[16];

    trigreturn_time = igris::millis() - trigstart_time;
    system_lock();
    serial6.write("ST", 2);

    igris_i32toa(value, buf, 10);
    serial6.write(buf, strlen(buf));

    serial6.write("UV", 2);

    igris_i32toa(number, buf, 10);
    serial6.write(buf, strlen(buf));

    system_unlock();
};

void scan_stop_send()
{
    serial6.write("JKLM", 4);
};

void scan_error_send()
{
    serial6.write("NOPR", 4);
};

void kill_and_remove(MoveController *controller)
{
    controller->remove_impl();
    controller->kill();
}

void stop_x()
{
    if (current_controller_x)
    {
        kill_and_remove(current_controller_x);
        current_controller_x = nullptr;
    }
    else
        axis_x.generator = 0;
};

void stop_y()
{
    if (current_controller_y)
    {
        kill_and_remove(current_controller_y);
        current_controller_y = nullptr;
    }
    else
        axis_y.generator = 0;
};

void stop()
{
    stop_x();
    stop_y();
};

void set_scan_cmpctr_x()
{
    stop_x();

    install_controller(&current_controller_x, &periodic_compare_x);
}

void set_scan_cmpctr_y()
{
    stop_y();

    install_controller(&current_controller_y, &periodic_compare_y);
}

void impulse_x(int32_t impulse)
{
    if (impulse > 0)
    {
        axis_x.set_direction(DIRECTION_FORWARD);
        axis_x.set_generator(abs(impulse));
    }
    else
    {
        axis_x.set_direction(DIRECTION_BACKWARD);
        axis_x.set_generator(abs(impulse));
    }
}

void impulse_y(int32_t impulse)
{
    if (impulse > 0)
    {
        axis_y.set_direction(DIRECTION_FORWARD);
        axis_y.set_generator(abs(impulse));
    }
    else
    {
        axis_y.set_direction(DIRECTION_BACKWARD);
        axis_y.set_generator(abs(impulse));
    }
}

void speed_x(uint32_t speed)
{
    axis_x.speed = speed;
    axis_x.update_speed();
};

void speed_y(uint32_t speed)
{
    axis_y.speed = speed;
    axis_y.update_speed();
};

void numer_gen_x(uint32_t numer)
{
    axis_x.gen_numer = numer;
    axis_x.update_speed();
};

void numer_gen_y(uint32_t numer)
{
    axis_y.gen_numer = numer;
    axis_y.update_speed();
};

void denum_gen_x(uint32_t denum)
{
    axis_x.gen_denum = denum;
    axis_x.update_speed();
};

void denum_gen_y(uint32_t denum)
{
    axis_y.gen_denum = denum;
    axis_y.update_speed();
};

void spdmul_numer_x(uint32_t spdmul_numer)
{
    axis_x.spdmul_numer = spdmul_numer;
    axis_x.update_speed();
}

void spdmul_denum_x(uint32_t spdmul_denum)
{
    axis_x.spdmul_denum = spdmul_denum;
    axis_x.update_speed();
}

void spdmul_numer_y(uint32_t spdmul_numer)
{
    axis_y.spdmul_numer = spdmul_numer;
    axis_y.update_speed();
}

void spdmul_denum_y(uint32_t spdmul_denum)
{
    axis_y.spdmul_denum = spdmul_denum;
    axis_y.update_speed();
}

void direction_x(Direction dir)
{
    axis_x.set_direction(dir);
};

void direction_y(Direction dir)
{
    axis_y.set_direction(dir);
};

void set_incpos_move_x(int32_t val)
{
    if (val >= 0)
        periodic_compare_x.set_incpos_direction(DIRECTION_FORWARD);
    else
        periodic_compare_x.set_incpos_direction(DIRECTION_BACKWARD);
    periodic_compare_x.set_incpos_move(abs(val));
}

void set_incpos_move_y(int32_t val)
{
    if (val >= 0)
        periodic_compare_y.set_incpos_direction(DIRECTION_FORWARD);
    else
        periodic_compare_y.set_incpos_direction(DIRECTION_BACKWARD);
    periodic_compare_y.set_incpos_move(abs(val));
}

void set_incpos_scan_stop_x(int32_t val)
{
    periodic_compare_x.set_incpos_scan_stop_zone(val);
}
void set_incpos_scan_stop_y(int32_t val)
{
    periodic_compare_y.set_incpos_scan_stop_zone(val);
}

void set_incpos_scan_strt_x(int32_t val)
{
    periodic_compare_x.set_incpos_scan_strt_zone(val);
}

void set_incpos_scan_strt_y(int32_t val)
{
    periodic_compare_y.set_incpos_scan_strt_zone(val);
}

void set_incpos_points_x(int32_t val)
{
    periodic_compare_x.set_incpos_points(val);
}
void set_incpos_points_y(int32_t val)
{
    periodic_compare_y.set_incpos_points(val);
}

void set_trigin_divider_x(int32_t val)
{
    trigin_divider = val;
}

void set_trigin_divider_y(int32_t val)
{
    trigin_divider = val;
}

void set_waittrigmode(int32_t value)
{
    if (value == 2)
    {
        disable_trigin_mode = 1;
    }

    else
    {
        disable_trigin_mode = 0;
    }

    waittrigmode = (bool)value;
}

volatile bool notify_on_answer_pulse = false;

void trigout_send_pulse()
{
    periodic_compare_x.set_trigwait_state(true);
    notify_on_answer_pulse = true;
    trigstart_time = igris::millis();
    stm32_timer_enable(TIM10, true);
}

int GLOBAL_scan_command_counter = 0;
void trigout_send_pulse_fast_notify()
{
    periodic_compare_x.set_trigwait_state(false);
    notify_on_answer_pulse = false;
    trigstart_time = igris::millis();
    stm32_timer_enable(TIM10, true);
    GLOBAL_scan_command_counter++;
    scan_command_send(GLOBAL_scan_command_counter);
}

void trigout_send_pulse_without_notify()
{
    // periodic_compare_x.trigwait = true;
    stm32_timer_enable(TIM10, true);
    // notify_on_answer_pulse = false;
}

void switch_mode_state0()
{
    set_discrete_output_unical(0);
}

void switch_mode_state1()
{
    set_discrete_output_unical(1);
}

void switch_mode_state0_aug()
{
    system_lock();
    if (periodic_compare_x.runned() && periodic_compare_x.is_trigwait_state())
    {
        periodic_compare_x.set_trigwait_hook(switch_mode_state0);
    }
    else
    {
        switch_mode_state0();
    }
    system_unlock();
}

void switch_mode_state1_aug()
{
    system_lock();
    if (periodic_compare_x.runned() && periodic_compare_x.is_trigwait_state())
    {
        periodic_compare_x.set_trigwait_hook(switch_mode_state1);
    }
    else
    {
        switch_mode_state1();
    }
    system_unlock();
}

// For manual discrete output control.
// Set discrete output with number @num in state @state.
// @state : 1-high level. 0-low level.
void set_discrete_output_state(int num, int state)
{
    if (num >= COUNT_OF_DISCRETE_OUTPUTS)
        return;

    extpin[num].set(state);
}

// Activate one of output states and disable each other.
void set_discrete_output_unical(int num)
{
    if (num >= COUNT_OF_DISCRETE_OUTPUTS)
    {
        disable_all_discrete_outputs();
        return;
    }

    for (int i = 0; i < COUNT_OF_DISCRETE_OUTPUTS; ++i)
    {
        if (i != num)
        {
            extpin[i].set(0);
        }
    }

    extpin[num].set(1);
}

void disable_all_discrete_outputs()
{
    for (int i = 0; i < COUNT_OF_DISCRETE_OUTPUTS; ++i)
    {
        extpin[i].set(0);
    }
}
