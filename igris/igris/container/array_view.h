#ifndef IGRIS_CONTAINER_ARRAY_VIEW_H
#define IGRIS_CONTAINER_ARRAY_VIEW_H

#include <initializer_list>
#include <stdlib.h>
#include <utility>

namespace igris
{
    template <class T> class array_view
    {
        T *_data = nullptr;
        size_t _size = 0;

    public:
        using iterator = T *;
        using const_iterator = const T *;
        using value_type = T;

    public:
        array_view(T *data, size_t size) : _data(data), _size(size) {}
        array_view(T *data, int size) : _data(data), _size((size_t)size) {}
        array_view() : _data(nullptr), _size(0) {}

        array_view(const std::initializer_list<T> &lst)
        {
            _data = (T *)std::begin(lst);
            _size = lst.size();
        }

        template <class C> array_view(C &c) : _data(c.data()), _size(c.size())
        {
        }
        template <class C>
        array_view(const C &c) : _data(c.data()), _size(c.size())
        {
        }

        template <size_t N> array_view(T (&arr)[N]) : _data(arr), _size(N) {}

        array_view &operator=(std::nullptr_t null)
        {
            _data = null;
            _size = 0;
            return *this;
        }

        const T *data() const
        {
            return _data;
        }

        T *data()
        {
            return _data;
        }

        size_t size() const
        {
            return _size;
        }

        T *begin()
        {
            return _data;
        }
        T *end()
        {
            return _data + _size;
        }
        const T *begin() const
        {
            return _data;
        }
        const T *end() const
        {
            return _data + _size;
        }

        T *rbegin()
        {
            return _data + _size - 1;
        }
        T *rend()
        {
            return _data - 1;
        }
        const T *rbegin() const
        {
            return _data + _size - 1;
        }
        const T *rend() const
        {
            return _data - 1;
        }

        T &operator[](int i)
        {
            return *(_data + i);
        }
        const T &operator[](int i) const
        {
            return *(_data + i);
        }

        array_view slice(int start, int size)
        {
            return array_view(_data + start, size);
        }

        /*template <class U>
        operator array_view<U>()
        {
            static_assert(std::is_convertible<T,U>::value);
            return reinterpret_cast<array_view<U>>(this);
        }*/
    };
}

#endif
