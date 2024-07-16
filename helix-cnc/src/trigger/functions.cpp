#include "functions.h"
#include <cmath>
#include <cstring>
#include <igris/defs/io.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>
#include <trigger/axis.h>
#include <trigger/comparectr.h>
#include <trigger/global.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/timer.h>

#define COUNT_OF_DISCRETE_OUTPUTS 4

extern struct gpio_pin extpin[4];

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
    axis_x.reset_encoder();
};

void reset_encoder_y()
{
    axis_y.reset_encoder();
};

int32_t trigin_count()
{
    return trig_in_counter;
};

//Ручная активация callback trigger для работы с nac_pulse.
void manual_trig_callback(bool en)
{
    stop();

    eventctr_mode = false;
    disable_trigin_mode = 0;

    if (en)
        trigin_handler = scan_command_send;
    else
        trigin_handler = {};
};

void scan_command_send()
{
    char buf[16];
    igris_i32toa(periodic_compare_x.triggers_points, buf, 10);

    trigreturn_time = igris::millis() - trigstart_time;
    system_lock();
    // TODO
    // serial6.write("WXYZ", 4);
    // serial6.write(buf, strlen(buf));
    system_unlock();
};

void scan_stop_send(){
    // TODO
    // serial6.write("JKLM", 4);
};

void scan_error_send(){
    // TODO
    // serial6.write("NOPR", 4);
};

void stop_x()
{
    if (periodic_compare_x.runned())
        periodic_compare_x.finish();
};

void stop_y()
{
    if (periodic_compare_y.runned())
        periodic_compare_y.finish();
};

void stop()
{
    stop_x();
    stop_y();
};

void set_scan_cmpctr_x()
{
    periodic_compare_x.start();
}

void set_scan_cmpctr_y()
{
    periodic_compare_y.start();
}

void spdmul_numer_x(uint32_t spdmul_numer)
{
    axis_x.spdmul_numer = spdmul_numer;
    // axis_x.update_speed();
}

void spdmul_denum_x(uint32_t spdmul_denum)
{
    axis_x.spdmul_denum = spdmul_denum;
    // axis_x.update_speed();
}

void spdmul_numer_y(uint32_t spdmul_numer)
{
    axis_y.spdmul_numer = spdmul_numer;
    // axis_y.update_speed();
}

void spdmul_denum_y(uint32_t spdmul_denum)
{
    axis_y.spdmul_denum = spdmul_denum;
    // axis_y.update_speed();
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
        periodic_compare_x.incpos_direction = DIRECTION_FORWARD;
    else
        periodic_compare_x.incpos_direction = DIRECTION_BACKWARD;
    periodic_compare_x.incpos_move = std::abs(val);
}

void set_incpos_move_y(int32_t val)
{
    if (val >= 0)
        periodic_compare_y.incpos_direction = DIRECTION_FORWARD;
    else
        periodic_compare_y.incpos_direction = DIRECTION_BACKWARD;
    periodic_compare_y.incpos_move = std::abs(val);
}

void set_incpos_scan_stop_x(int32_t val)
{
    periodic_compare_x.incpos_scan_stop_zone = val;
}
void set_incpos_scan_stop_y(int32_t val)
{
    periodic_compare_y.incpos_scan_stop_zone = val;
}

void set_incpos_scan_strt_x(int32_t val)
{
    periodic_compare_x.incpos_scan_strt_zone = val;
}
void set_incpos_scan_strt_y(int32_t val)
{
    periodic_compare_y.incpos_scan_strt_zone = val;
}

void set_incpos_points_x(int32_t val)
{
    periodic_compare_x.incpos_points = val;
}
void set_incpos_points_y(int32_t val)
{
    periodic_compare_y.incpos_points = val;
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

void set_bitrigmode(int32_t value)
{
    periodic_compare_x.bitrigmode = (bool)value;
}

void trigout_send_pulse()
{
    periodic_compare_x.trigwait = true;
    notify_on_answer_pulse = true;
    trigstart_time = igris::millis();
    stm32_timer_enable(TIM10, true);
}

void trigout_send_pulse_fast_notify()
{
    periodic_compare_x.trigwait = false;
    notify_on_answer_pulse = false;
    trigstart_time = igris::millis();
    stm32_timer_enable(TIM10, true);
    scan_command_send();
}

void trigout_send_pulse_without_notify()
{
    // periodic_compare_x.trigwait = true;
    stm32_timer_enable(TIM10, true);
    // notify_on_answer_pulse = false;
}

#ifdef BOARD_RL_SYNCRO_V2
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
    if (periodic_compare_x.runned() && periodic_compare_x.trigwait)
    {
        periodic_compare_x.trigwait_hook = switch_mode_state0;
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
    if (periodic_compare_x.runned() && periodic_compare_x.trigwait)
    {
        periodic_compare_x.trigwait_hook = switch_mode_state1;
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

#endif
