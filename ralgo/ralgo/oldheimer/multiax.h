#ifndef RALGO_HEIMER_MULTIAX_H
#define RALGO_HEIMER_MULTIAX_H

#include <ralgo/oldheimer/axis.h>

namespace heimer
{
    template <class P, class V>
    class virtual_axis_node : public heimer::axis_node<P, V>
    {
        using axnode = heimer::axis_node<P, V>;

        control_node *parent;

    public:
        virtual_axis_node(const char *mnemo, control_node *parent)
            : axis_node<P, V>(mnemo), parent(parent)
        {
        }

        int on_activate() override
        {
            return HEIM_ERR_IS_PARTED;
        }

        P request_feedback_position() override
        {
            // Должна обновляться управляющим устройством
            return heimer::axis_node<P, V>::feedpos;
        }

        virtual bool on_interrupt(control_node *slave,
                                  control_node *source,
                                  interrupt_args *data)
        {
            if (data->code() == HEIMER_INTERRUPT_TYPE_CONTROL_UPDATE)
            {
                axnode::ctrpos = axnode::feedpos;
                axnode::ctrspd = axnode::feedspd;
            }

            return false; // пробросить выше
        }
    };
}

#endif
