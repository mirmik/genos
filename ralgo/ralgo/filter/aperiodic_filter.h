/**
    @file
*/

#ifndef RALGO_FILTER_APERIODIC_FILTER_H
#define RALGO_FILTER_APERIODIC_FILTER_H

#include <ralgo/filter/inout.h>

namespace ralgo
{
    template <class T> class aperiodic_filter : public ralgo::inout<T, T>
    {
        float koeff = 0;
        float timeconst = 1;
        float invert_timeconst = 1;
        T state = {};

    public:
        aperiodic_filter() {}

        aperiodic_filter(float _koeff) : koeff(_koeff) {}

        aperiodic_filter(float delta, float time_constant)
        {
            set_koefficient(delta, time_constant);
        }

        T serve(const T &in)
        {
            state += (in - state) * (double)koeff;
            return state;
        }

        T serve(const T &in, float delta)
        {
            state += (in - state) * (delta / timeconst);
            return state;
        }

        T operator()(const T &in) override
        {
            return serve(in);
        }

        void reset(T val)
        {
            state = val;
        }

        aperiodic_filter &set_koefficient(float delta, float time_constant)
        {
            koeff = delta / time_constant;
            timeconst = time_constant;
            invert_timeconst = 1 / time_constant;
            return *this;
        }

        aperiodic_filter &set_timeconst(float time_constant)
        {
            timeconst = time_constant;
            invert_timeconst = 1 / time_constant;
            return *this;
        }
    };
}

#endif
