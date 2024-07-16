#ifndef RALGO_HEIMER_PHASE_MUX_PROCESSOR_H
#define RALGO_HEIMER_PHASE_MUX_PROCESSOR_H

#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/phase_signals.h>
#include <ralgo/heimer/signal_processor.h>

namespace heimer
{
    template <int Dim> class phase_mux_processor : public signal_processor
    {
        phase_signal<Dim> *leftside = nullptr;
        axis_state *rightside[Dim] = {};

    public:
        phase_mux_processor(const std::string &name)
            : signal_processor(name, 1, Dim)
        {
        }

        phase_mux_processor(const phase_mux_processor &) = delete;
        phase_mux_processor &operator=(const phase_mux_processor &) = delete;

        int feedback(disctime_t)
        {
            for (int i = 0; i < Dim; ++i)
            {
                rightside[i]->feedpos = leftside->feedpos[i];
                rightside[i]->feedvel = leftside->feedvel[i];
            }
            return 0;
        }

        int serve(disctime_t)
        {
            for (int i = 0; i < Dim; ++i)
            {
                leftside->ctrpos[i] = rightside[i]->ctrpos;
                leftside->ctrvel[i] = rightside[i]->ctrvel;
            }
            return 0;
        }

        signal_head *leftsig(int)
        {
            return leftside;
        }
        signal_head *rightsig(int i)
        {
            return rightside[i];
        }
        void set_leftsig(int, signal_head *it)
        {
            leftside = static_cast<phase_signal<Dim> *>(it);
        }
        void set_rightsig(int i, signal_head *it)
        {
            rightside[i] = static_cast<axis_state *>(it);
        }
        int leftsigtype(int)
        {
            return SIGNAL_TYPE_PHASE_SIGNAL_BASE + Dim - 1;
        }
        int rightsigtype(int)
        {
            return SIGNAL_TYPE_AXIS_STATE;
        }

        void on_activate(disctime_t)
        {
            for (int i = 0; i < Dim; ++i)
            {
                rightside[i]->ctrpos = rightside[i]->feedpos;
                rightside[i]->ctrvel = rightside[i]->feedvel;
            }
        }
    };
}

#endif
