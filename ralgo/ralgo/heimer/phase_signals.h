#ifndef RALGO_HEIMER_PHASE_SIGNALS_H
#define RALGO_HEIMER_PHASE_SIGNALS_H

#include <errno.h>

#include <nos/fprint.h>
#include <ralgo/heimer/heimer_types.h>
#include <ralgo/heimer/signal.h>
#include <ralgo/heimer/sigtypes.h>

#include <ralgo/linalg/linalg.h>
#include <ralgo/space/pose3.h>
#include <ralgo/space/screw.h>

namespace heimer
{
    template <class P, class V> class phase_signal_base : public signal_head
    {
    public:
        P ctrpos = {};
        V ctrvel = {};

        P feedpos = {};
        V feedvel = {};

    public:
        phase_signal_base(uint8_t type) : signal_head(type) {}

        phase_signal_base(const std::string &name, uint8_t type)
            : signal_head(name, type)
        {
        }

        ~phase_signal_base() override = default;

        int info(char *data, int) override
        {
            nos::format_buffer(data,
                               "(cpos:{}, cvel:{}, fpos:{}, fvel:{})\r\n",
                               ctrpos,
                               ctrvel,
                               feedpos,
                               feedvel);
            return 0;
        }
    };
}

#define PHASE_SIGNAL_CLASS(clsname, postype, veltype, typeno)                  \
    class clsname : public phase_signal_base<postype, veltype>                 \
    {                                                                          \
    public:                                                                    \
        clsname(const char *name)                                              \
            : phase_signal_base<postype, veltype>(name, typeno){};             \
    };

namespace heimer
{
    using posvec2 = linalg::vec<position_t, 2>;
    using velvec2 = linalg::vec<velocity_t, 2>;
    using posvec3 = linalg::vec<position_t, 3>;
    using velvec3 = linalg::vec<velocity_t, 3>;
    using pospose3 = ralgo::pose3<position_t>;
    using velscr3 = ralgo::screw3<velocity_t>;

    //	PHASE_SIGNAL_CLASS(axis_state, position_t, velocity_t,
    // SIGNAL_TYPE_AXIS_STATE) PHASE_SIGNAL_CLASS(dof6_state, pospose3, velscr3,
    // SIGNAL_TYPE_DOF6)

    template <int Dim>
    class phase_signal : public phase_signal_base<linalg::vec<position_t, Dim>,
                                                  linalg::vec<velocity_t, Dim>>
    {
    public:
        phase_signal(const char *name)
            : phase_signal_base<linalg::vec<position_t, Dim>,
                                linalg::vec<velocity_t, Dim>>(
                  name, SIGNAL_TYPE_PHASE_SIGNAL_BASE + Dim - 1){};

        phase_signal(const std::string &name)
            : phase_signal_base<linalg::vec<position_t, Dim>,
                                linalg::vec<velocity_t, Dim>>(
                  name, SIGNAL_TYPE_PHASE_SIGNAL_BASE + Dim - 1){};
    };

    using phase2_state = phase_signal<2>;
    using phase3_state = phase_signal<3>;
    using phase4_state = phase_signal<4>;
}

#endif
