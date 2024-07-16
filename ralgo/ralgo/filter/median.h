#ifndef RALGO_INOUT_MEDIAN_H
#define RALGO_INOUT_MEDIAN_H

#include <ralgo/inout/inout.h>

namespace ralgo
{
    template <typename T> class median3 : public ralgo::inout<T>
    {
        T x[3];
        uint8_t cursor = 0;

    public:
        median3(T initstate = T()) : x{initstate, initstate, initstate} {}

        T operator()(T in)
        {
            x[cursor++] = in;
            if (cursor == 3)
                cursor = 0;

            bool x01 = x[0] > x[1];
            bool x02 = x[0] > x[2];
            bool x12 = x[1] > x[2];

            return x01 ? (x12 ? x[1] : x[2]) : (x02 ? x[0] : x[2]);
        }
    };
}

#endif
