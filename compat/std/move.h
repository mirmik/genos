#ifndef GXX_STD_MOVE_H
#define GXX_STD_MOVE_H
#include "igris_std_config.hpp"

#include <type_traits>
#include "type_traits_impl/standalone.h"

namespace IGRIS_STD_NS
{
    template <class T>
    constexpr typename IGRIS_STD_NS::remove_reference<T>::type &&move(T &&t)
    {
        return static_cast<typename remove_reference<T>::type &&>(t);
    }

    template <class S>
    constexpr S &&forward(typename remove_reference<S>::type &a) noexcept
    {
        return static_cast<S &&>(a);
    }

    template <class S>
    constexpr S &&forward(typename remove_reference<S>::type &&a) noexcept
    {
        return static_cast<S &&>(a);
    }
}

#endif
