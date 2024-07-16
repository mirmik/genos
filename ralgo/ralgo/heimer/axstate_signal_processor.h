/** @file */

#ifndef RALGO_HEIMER_AXSTATE_SIGNAL_PROCESSOR_H
#define RALGO_HEIMER_AXSTATE_SIGNAL_PROCESSOR_H

#include <ralgo/heimer/axis_state.h>
#include <ralgo/heimer/signal_processor.h>

namespace heimer
{
    class axstate_signal_processor : public signal_processor
    {
    private:
        axis_state **_leftside = nullptr;
        axis_state **_rightside = nullptr;

    public:
        axstate_signal_processor() = default;
        axstate_signal_processor(const char *, int ldim, int rdim);
        axstate_signal_processor(const axstate_signal_processor &) = delete;
        axstate_signal_processor &
        operator=(const axstate_signal_processor &) = delete;

        void attach_leftside_table(axis_state **table);
        void attach_rightside_table(axis_state **table);

        axis_state *leftax(int i)
        {
            return _leftside[i];
        }
        axis_state *rightax(int i)
        {
            return _rightside[i];
        }

        int leftsigtype(int)
        {
            return SIGNAL_TYPE_AXIS_STATE;
        }
        int rightsigtype(int)
        {
            return SIGNAL_TYPE_AXIS_STATE;
        }

        signal_head *leftsig(int i)
        {
            return _leftside[i];
        }
        signal_head *rightsig(int i)
        {
            return _rightside[i];
        }

        void set_leftsig(int i, signal_head *sig)
        {
            _leftside[i] = static_cast<axis_state *>(sig);
        }
        void set_rightsig(int i, signal_head *sig)
        {
            _rightside[i] = static_cast<axis_state *>(sig);
        }

        void attach_axes_from_tables()
        {
            for (int i = 0; i < leftdim(); ++i)
            {
                leftsig(i)->attach_possible_controller(this);
            }

            for (int i = 0; i < rightdim(); ++i)
            {
                rightsig(i)->attach_listener(this);
            }
        }
    };
}

#endif
