/**
    @file position_applier.h
*/

#ifndef RALGO_HEIMER_POSITION_APPLIER
#define RALGO_HEIMER_POSITION_APPLIER

#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/phase_signals.h>
#include <ralgo/heimer/signal_processor.h>
#include <ralgo/robo/iposvel.h>

namespace heimer
{
    class position_applier : public signal_processor
    {
        robo::i_position_setter *posset = nullptr;
        robo::i_position_feedback *posget = nullptr;
        axis_state *state = nullptr;

    public:
        position_applier(const char *name,
                         robo::i_position_driver *driver,
                         axis_state *state)
            : signal_processor(name, 0, 1)
        {
            posset = driver;
            posget = driver;
            this->state = state;
        }

        position_applier(const position_applier &) = default;
        position_applier &operator=(const position_applier &) = default;

        position_t offset = {};
        position_t scale = {};

        int serve(disctime_t)
        {
            posset->set_position(state->ctrpos);

            return 0;
        }

        int feedback(disctime_t)
        {
            state->feedvel = state->ctrvel;
            state->feedpos = posget->feedback_position();

            return 0;
        }

        signal_head *iterate_left(signal_head *)
        {
            return NULL;
        }

        signal_head *iterate_right(signal_head *iter)
        {
            if (iter)
                return NULL;
            else
                return state;
        }
    };
}

#endif
