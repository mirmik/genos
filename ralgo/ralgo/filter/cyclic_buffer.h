#ifndef RALGO_CYCLIC_BUFFER_H
#define RALGO_CYCLIC_BUFFER_H

#include <igris/datastruct/ring_counter.h>

namespace ralgo
{
    template <class T, int N> class cyclic_buffer
    {
    public:
        T data[N];
        ring_counter counter;

        cyclic_buffer()
        {
            memset(data, 0, N * sizeof(T));
            ring_counter_init(&counter, N);
        }

        int size()
        {
            return N;
        }

        T push(T val)
        {
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
    };
}

#endif
