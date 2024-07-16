#ifndef RALGO_HEIMER_AXIS_STATE_H
#define RALGO_HEIMER_AXIS_STATE_H

#include <ralgo/heimer/heimer_types.h>
#include <ralgo/heimer/phase_signals.h>

namespace heimer
{
    class axis_state : public phase_signal_base<position_t, velocity_t>
    {
    public:
        axis_state()
            : phase_signal_base<position_t, velocity_t>(
                  SIGNAL_TYPE_AXIS_STATE){};

        axis_state(const std::string &name)
            : phase_signal_base<position_t, velocity_t>(
                  name, SIGNAL_TYPE_AXIS_STATE){};

        int command_v(int argc, char **argv, char *output, int maxsize) override
        {
            int status = ENOENT;

            if (strcmp("setpos", argv[0]) == 0)
            {
                feedpos = atof(argv[1]);
                ctrpos = atof(argv[1]);
                return 0;
            }

            if (status != ENOENT)
                return status;

            return signal_head::command_v(argc, argv, output, maxsize);
        }

        ~axis_state() override = default;
    };
}

#endif
