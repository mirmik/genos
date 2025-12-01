#ifndef RALGO_CYCLIC_BUFFER_H
#define RALGO_CYCLIC_BUFFER_H

#include <igris/container/unbounded_array.h>
#include <igris/datastruct/ring_counter.h>

namespace igris
{
    template <class T, class Alloc = std::allocator<T>> class cyclic_buffer
    {
    public:
        igris::unbounded_array<T, Alloc> data = {};
        ring_counter counter = {};
        size_t _size = 0;

        cyclic_buffer(size_t size) : data(size)
        {
            ring_counter_init(&counter, size);
        }

        size_t size() const
        {
            return _size;
        }

        T push(T val)
        {
            if (_size < (size_t)counter.size)
                _size++;

            ring_counter_increment(&counter, 1);
            T ret = data[ring_counter_get(&counter)];
            data[ring_counter_get(&counter)] = val;
            return ret;
        }

        /// Получить элемент z*i, где z - дискретный оператор.
        /// 0 - последний, 1 - предпоследний
        // и так далее
        T operator[](int i)
        {
            return data[ring_counter_prev(&counter, i)];
        }

        const T operator[](int i) const
        {
            return data[ring_counter_prev(&counter, i)];
        }

        void resize(size_t size)
        {
            data.resize(size);
            ring_counter_init(&counter, size);
        }
    };
}

#endif
