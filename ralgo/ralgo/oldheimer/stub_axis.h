#ifndef RALGO_HEIMER_STUB_AXIS_H
#define RALGO_HEIMER_STUB_AXIS_H

#include <ralgo/oldheimer/axis.h>

namespace heimer
{
    template <class P, class V> class stub_axis : public axis_node<P, V>
    {
        using parent = axis_node<P, V>;

    public:
        stub_axis(const char *name) : parent(name) {}

        void feedback()
        {
            parent::feedpos = parent::ctrpos;
            parent::feedspd = parent::ctrspd;
        }

        P request_feedback_position() override
        {
            // Должна обновляться управляющим устройством
            return axis_node<P, V>::feedpos;
        }

        void serve() {}
    };
}

#endif
