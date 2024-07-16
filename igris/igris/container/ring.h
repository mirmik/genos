/// @file

#ifndef IGRIS_RING_H
#define IGRIS_RING_H

#include <igris/container/unbounded_array.h>
#include <igris/datastruct/ring.h>
#include <memory>
#include <new>
#include <utility>
#include <vector>

namespace igris
{
    /**
     * Обёртка над api ring_head, предназначенная для управления буфером
     * указанного типа.
     * С целью оптимизации разрешает доступ к внутренним переменным и прямое
     * управление буффером.
     * */
    template <typename T, class Alloc = std::allocator<T>> class ring
    {
    public:
        ring_head r = {};
        unbounded_array<T, Alloc> buffer = {};

    public:
        ring() = default;

        ring(int bufsize) : buffer(bufsize + 1)
        {
            ring_init(&r, bufsize + 1);
        }

        void resize(size_t sz)
        {
            buffer.resize(sz);
            ring_init(&r, sz + 1);
        }

        void reset()
        {
            ring_init(&r, buffer.size());
        }

        std::vector<T> get_last(int offset, int count, bool order_from_end)
        {
            std::vector<T> vec(count);

            if (order_from_end)
            {
                for (int i = 0; i < count; ++i)
                {
                    vec[i] = buffer[fixup_index(r.head - offset - i - 1)];
                }
            }
            else
            {
                for (int i = 0; i < count; ++i)
                {
                    vec[i] = buffer[fixup_index(r.head - count - offset + i)];
                }
            }

            return vec;
        }

        size_t read(T *buf, size_t sz)
        {
            size_t a = ring_read(&r, buffer.data(), buf, sz);
            return a;
        }

        size_t write(const T *buf, size_t sz)
        {
            size_t a = ring_write(&r, buffer.data(), buf, sz);
            return a;
        }

        __ALWAYS_INLINE inline bool empty()
        {
            return ring_empty(&r);
        }

        void push(const T &obj)
        {
            new (buffer.data() + r.head) T(obj);
            ring_move_head_one(&r);
        }

        template <typename... Args> void emplace(Args &&... args)
        {
            new (buffer.data() + r.head) T(std::forward<Args>(args)...);
            ring_move_head_one(&r);
        }

        void clear()
        {
            while (!empty())
            {
                pop();
            }
        }

        void pop()
        {
            int idx = r.tail;
            buffer[idx].~T();
            ring_move_tail_one(&r);
        }

        __ALWAYS_INLINE
        void move_tail_one()
        {
            ring_move_tail_one(&r);
        }

        void move_head_one()
        {
            ring_move_head_one(&r);
        }

        unsigned int avail()
        {
            return ring_avail(&r);
        }

        unsigned int room()
        {
            return ring_room(&r);
        }

        unsigned int size()
        {
            return r.size;
        }

        T &get(int index)
        {
            return buffer[index];
        }

        __ALWAYS_INLINE inline T &tail()
        {
            return buffer[r.tail];
        }

        __ALWAYS_INLINE inline T &last()
        {
            int idx = fixup_index(r.head - 1);
            return buffer[idx];
        }

        // Determine element index in current ring array.
        int index_of(T *element)
        {
            return element - buffer.data();
        }

        int tail_index()
        {
            return r.tail;
        }

        int head_index()
        {
            return r.head;
        }

        void set_last_index(int idx)
        {
            r.head = idx;
            move_head_one();
        }

        int fixup_index(int index)
        {
            return ring_fixup_index(&r, index);
        }

        T &head_place()
        {
            return buffer[r.head];
        }

        int distance(int a, int b)
        {
            return (a - b + r.size) % r.size;
        }
    };
}

#endif
