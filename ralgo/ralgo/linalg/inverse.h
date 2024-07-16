#ifndef RALGO_LINALG_INVERSE_H
#define RALGO_LINALG_INVERSE_H

#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/plud.h>
#include <ralgo/util/helpers.h>

namespace ralgo
{
    template <class R = void, class A>
    defsame_t<R, ralgo::matrix<typename A::value_type>> inverse(const A &a)
    {
        auto plud = ralgo::plud(a);
        auto inv = plud.inverse();
        return inv;
    }
}

#endif