#ifndef IGRIS_STD_UTILITY_MINIMAL_H
#define IGRIS_STD_UTILITY_MINIMAL_H

#include <type_traits_impl/standalone.h>

namespace std
{
    // std::declval
    template <class T>
    typename std::add_rvalue_reference<T>::type declval() noexcept;

    // std::forward
    template <class T>
    constexpr T &&forward(typename std::remove_reference<T>::type &t) noexcept;

    template <class T>
    constexpr T &&forward(typename std::remove_reference<T>::type &&t) noexcept;
}

#endif
