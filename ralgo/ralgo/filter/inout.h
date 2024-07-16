/**
    @file
*/

#ifndef RALGO_INOUT_H
#define RALGO_INOUT_H

namespace ralgo
{
    template <class R, class V> struct inout
    {
        virtual R operator()(const V &in) = 0;
    };
}

#endif
