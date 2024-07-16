#ifndef RALGO_HEIMER_DOF6_SIGNAL_H
#define RALGO_HEIMER_DOF6_SIGNAL_H

#include <ralgo/space/pose3.h>
#include <ralgo/space/screw.h>

#include <ralgo/heimer/heimer_types.h>
#include <ralgo/heimer/signal.h>

namespace heimer
{
    class dof6_signal : public signal_head
    {
        ralgo::pose3<position_t> ctrpos = {};
        ralgo::screw3<velocity_t> ctrvel = {};
        ralgo::pose3<position_t> feedpos = {};
        ralgo::screw3<velocity_t> feedvel = {};

    public:
        dof6_signal(const char *name);
        int info(char *data, int maxsize) override;
    };
}

#endif
