#include <ralgo/robo/stepper_controller.h>

#include <igris/math.h>
#include <math.h>
#include <ralgo/disctime.h>

#include <igris/dprint.h>
#include <nos/fprint.h>

#include <ralgo/log.h>

using namespace robo;

stepper_controller::stepper_controller()
{
    _control_pos = 0;
    _virtual_pos = 0;
}

stepper_controller::stepper_controller(robo::stepper *stepper)
{
    init(stepper);
}

void stepper_controller::init(robo::stepper *stepper)
{
    this->stepper = stepper;

    _control_pos = 0;
    _virtual_pos = 0;
}

void stepper_controller::set_steps_position(position_t pos)
{
    _control_pos = pos * units_in_step;
    _virtual_pos = pos * units_in_step;
}

int stepper_controller::shift(int64_t shift)
{
    if (ABS(shift) >= units_in_step)
    {
        ralgo::warn("overrun ");
        return STEPCTR_OVERRUN;
    }

    _virtual_pos += shift;
    int64_t diffpos = _virtual_pos - _control_pos;

    if (diffpos > 0)
    {
        if (diffpos > units_in_step_triggered)
        {
            stepper->inc();
            _control_pos += units_in_step;
        }
    }

    else
    {
        if (diffpos < -units_in_step_triggered)
        {
            stepper->dec();
            _control_pos -= units_in_step;
        }
    }

    return 0;
}

int stepper_controller::speed_apply(double speed, double delta
                                    // дискретное время дано с плавающей точкой,
                                    // чтобы можно было передавать интервалы
                                    // времени меньше disctime

)
{
    int64_t arg = speed * delta;
    return shift(arg);
}

int fixed_frequency_stepper_controller::constant_frequency_serve()
{
    int sts = shift(current_shift);

    if (sts)
    {
        if (interrupt_handle)
            interrupt_handle(interrupt_priv, sts);
    }

    return sts;
}

fixed_frequency_stepper_controller::fixed_frequency_stepper_controller(
    robo::stepper *stepper)
    : stepper_controller(stepper)
{
}

fixed_frequency_stepper_controller::fixed_frequency_stepper_controller()
    : stepper_controller()
{
}

void fixed_frequency_stepper_controller::set_velocity(double speed)
{
    auto val = speed_to_shift * speed;

    system_lock();
    this->current_shift = val;
    system_unlock();
}

void stepper_controller::info(char *, int)
{
    // nos::format_buffer(buf, "_control_pos: {}\r\n_virtual_pos:{}\r\n");
}

void fixed_frequency_stepper_controller::info(char *buf, int)
{
    // char sbuf[48];
    // stepper_controller::info(buf, len);

    nos::format_buffer(buf,
                       "speed_to_shift: {}*10**3\r\n"
                       "freq: {}\r\n"
                       "steps: {}\r\n"
                       "current_shift: {}\r\n"
                       "_control_pos: {}\r\n"
                       "_virtual_pos: {}\r\n"
                       "units_in_step_triggered: {}\r\n",
                       speed_to_shift / 1000,
                       freq,
                       feedback_position(),
                       current_shift,
                       _control_pos,
                       _virtual_pos,
                       units_in_step_triggered);

    // strcat(buf, sbuf);
}

/// Расчитывает множитель, переводящий
/// скорость (имп / сек) в сдвиг
void fixed_frequency_stepper_controller::evaluate()
{
    speed_to_shift = units_in_step / freq;
}
