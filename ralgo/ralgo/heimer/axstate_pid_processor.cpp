#include <ralgo/heimer/axstate_pid_processor.h>

heimer::axstate_pid_processor::axstate_pid_processor(const char *name)
    : signal_processor(name, 1, 1)
{
}

void heimer::axstate_pid_processor::set_compcoeff(double coeff)
{
    compcoeff = coeff;
}

void heimer::axstate_pid_processor::set_left(heimer::axis_state *iter)
{
    state = iter;
}

void heimer::axstate_pid_processor::set_right(heimer::scalar_signal *iter)
{
    target = iter;
}

heimer::signal_head *
heimer::axstate_pid_processor::iterate_left(heimer::signal_head *iter)
{
    return iter == nullptr ? state : nullptr;
}

heimer::signal_head *
heimer::axstate_pid_processor::iterate_right(heimer::signal_head *iter)
{
    return iter == nullptr ? target : nullptr;
}

int heimer::axstate_pid_processor::feedback(disctime_t)
{
    return 0;
}

int heimer::axstate_pid_processor::serve(disctime_t time)
{
    double error = target->value - state->feedpos;

    state->ctrvel = error * compcoeff;
    state->ctrpos = state->feedpos;

    last_serve_time = time;
    return 0;
}

int heimer::axstate_pid_processor::command(int, char **, char *, int)
{
    return -1;
}

void heimer::axstate_pid_processor::deinit()
{
    return;
}

void heimer::axstate_pid_processor::on_activate(disctime_t time)
{
    last_serve_time = time;
}
