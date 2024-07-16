#ifndef NOS_UTIL_SIZE_H
#define NOS_UTIL_SIZE_H

// Расширенная версия std::size для старых стандартов.
// В будущем удалить.

#include <utility>

namespace nos
{
    template <class C>
    [[deprecated]] constexpr auto size(const C &c) -> decltype(c.size())
    {
        return c.size();
    }

    template <class C>
    [[deprecated]] constexpr auto ssize(const C &c)
        -> decltype(c.size() - c.size())
    {
        using R = decltype(c.size() - c.size());
        return static_cast<R>(c.size());
    }

    template <class T, std::size_t N>
    [[deprecated]] constexpr std::size_t size(const T (&)[N]) noexcept
    {
        return N;
    }

    template <class T, int N>
    [[deprecated]] constexpr auto ssize(const T (&)[N]) noexcept
        -> decltype(std::size_t() - std::size_t())
    {
        return N;
    }
}

#endif
