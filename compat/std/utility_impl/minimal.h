#ifndef IGRIS_STD_UTILITY_MINIMAL_H
#define IGRIS_STD_UTILITY_MINIMAL_H
#include "../igris_std_config.hpp"

#include "../type_traits_impl/standalone.h"

namespace IGRIS_STD_NS
{
    // IGRIS_STD_NS::declval
    template <class T>
    typename IGRIS_STD_NS::add_rvalue_reference<T>::type declval() noexcept;

    // IGRIS_STD_NS::forward
    template <class T>
    constexpr T &&forward(typename IGRIS_STD_NS::remove_reference<T>::type &t) noexcept;

    template <class T>
    constexpr T &&forward(typename IGRIS_STD_NS::remove_reference<T>::type &&t) noexcept;
}

#endif
