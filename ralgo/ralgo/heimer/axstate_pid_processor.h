#ifndef RALGO_HEIMER_AXSTATE_PID_PROCESSOR_H
#define RALGO_HEIMER_AXSTATE_PID_PROCESSOR_H

#include <ralgo/heimer/axstate_signal_processor.h>
#include <ralgo/heimer/scalar_signal.h>

namespace heimer
{
    class axstate_pid_processor : public signal_processor
    {
        axis_state *state = nullptr;
        scalar_signal *target = nullptr;

        double compcoeff = 0;
        disctime_t last_serve_time = 0;

    public:
        axstate_pid_processor(const char *name);
        axstate_pid_processor(const axstate_pid_processor &) = default;
        axstate_pid_processor &
        operator=(const axstate_pid_processor &) = default;

        int feedback(disctime_t time) override;
        int serve(disctime_t time) override;
        int command(int argc, char **argv, char *output, int outmax) override;
        void deinit() override;

        void set_right(scalar_signal *);
        void set_left(axis_state *);
        void set_compcoeff(double coeff);

        signal_head *iterate_left(signal_head *iter) override;
        signal_head *iterate_right(signal_head *iter) override;

        void on_activate(disctime_t time) override;
    };
}

#endif
