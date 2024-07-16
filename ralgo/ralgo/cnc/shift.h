#ifndef RALGO_SHIFT_H
#define RALGO_SHIFT_H

#include <igris/container/static_vector.h>

typedef uint16_t revolver_t;

namespace cnc
{
    class control_shift
    {
    public:
        revolver_t step = {};
        revolver_t direction = {};

    public:
        control_shift() {}

        control_shift(revolver_t step, revolver_t direction)
            : step(step), direction(direction)
        {
        }
    };
}

#endif
