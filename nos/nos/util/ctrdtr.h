#ifndef NOS_UTIL_CTRDTR_H
#define NOS_UTIL_CTRDTR_H

#include <new>

namespace nos
{
    template <typename T> void destructor(T *ptr)
    {
        ptr->~T();
    }

    template <typename T, typename... Args>
    void constructor(T *ptr, Args &&... args)
    {
        new ((void *)ptr) T(std::forward<Args>(args)...);
    }

    template <typename T> void copy_constructor(T *ptr, const T &other)
    {
        new ((void *)ptr) T(other);
    }

    template <typename T> void move_constructor(T *ptr, T &&other)
    {
        new ((void *)ptr) T(std::move(other));
    }

    template <class InputIterator, class EndIterator>
    void array_destructor(InputIterator first, EndIterator last)
    {
        while (first != last)
        {
            nos::destructor(&*first);
            ++first;
        }
    }

    template <class InputIterator, class EndIterator, typename... Args>
    void array_constructor(InputIterator first, EndIterator last, Args... args)
    {
        while (first != last)
        {
            nos::constructor(&*first, args...);
            ++first;
        }
    }
}

#endif
