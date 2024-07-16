#ifndef HEIMER_AXIS_H
#define HEIMER_AXIS_H

#include <assert.h>
#include <math.h>
#include <ralgo/oldheimer/control.h>

namespace heimer
{
    template <class P, class V> class axis_node : public heimer::control_node
    {
    public:
        P feedpos = 0;
        V feedspd = 0;

        P ctrpos = 0;
        V ctrspd = 0;

        bool updated_flag = false;

        constexpr axis_node(const char *mnemo) : control_node(mnemo) {}
        virtual ~axis_node() = default;

        void print_info() override
        {
            nos::println("ctrpos:", ctrpos);
            nos::println("ctrspd:", ctrspd);
            nos::println("feedpos:", feedpos);
            nos::println("feedspd:", feedspd);
        }

        P feedback_position()
        {
            return feedpos;
        }
        P feedback_speed()
        {
            return feedspd;
        }

        P target_position()
        {
            return ctrpos;
        }
        P target_speed()
        {
            return ctrspd;
        }

        void restore_control(P pos, V spd)
        {
            ctrpos = pos;
            ctrspd = spd;

            feedpos = pos;
            feedspd = spd;
        }

        void set_current_phase(P pos, V spd)
        {
            ctrpos = feedpos = pos;
            ctrspd = feedspd = spd;
        }

        void control(P pos, V spd)
        {
            assert(!isnan(ctrspd));
            ctrpos = pos;
            ctrspd = spd;
        }

        virtual void hardstop()
        {
            ctrpos = feedpos;
            ctrspd = 0;
        }

        P position_error()
        {
            return ctrpos - feedpos;
        }

        virtual P request_feedback_position() = 0;

        bool on_interrupt(control_node *slave,
                          control_node *source,
                          interrupt_args *data) override
        {
            (void)slave;
            (void)source;
            if (data->code() == HEIMER_INTERRUPT_TYPE_CONTROL_UPDATE)
            {
                P error = position_error();

                feedpos = request_feedback_position();
                ctrpos = feedpos + error;
            }

            return false; // пробросить выше
        }

        void serve_impl() override
        {
            // do_nothing
        }
    };
}

#endif
