#ifndef RALGO_FLOW_WINDOW_H
#define RALGO_FLOW_WINDOW_H

#include <assert.h>
#include <string.h>

#include <igris/container/array_view.h>
#include <igris/dprint.h>

#include <algorithm>

// Массив для реализации алгоритмов скользящего окна.
//
// Описание алгоритма.
// Массив состоит из двух смежных (последовательно расположенных в памяти)
// буфферов, каждый из которых имеет размер окна. Новые данные пишутся в каждый
// буффер, то есть записываются дважды. Окно скользит по пространству обоих
// буфферов, постепенно переползая из первого буффера во второй. По достижению
// конца второго буфера, окно смещается к началу первого (в позицию begin+1).
// При этом, данные в окне выглядят так, как будто оно продолжает ползти по
// данным последовательно.

namespace ralgo
{
    template <class T> class sliding_array
    {
    public:
        size_t halfsize = {};
        size_t cursor = {};
        T *dataarr = nullptr;

    public:
        sliding_array(size_t size) : cursor(0), dataarr(nullptr)
        {
            resize(size);
        }
        sliding_array(const sliding_array &) = delete;
        sliding_array &operator=(const sliding_array &) = delete;

        void invalidate()
        {
            delete[] dataarr;
        }

        ~sliding_array()
        {
            invalidate();
        }

        void resize(size_t halfsize)
        {
            if (dataarr)
                delete[](dataarr);

            this->halfsize = halfsize;
            dataarr = new T[halfsize * 2];
            memset(dataarr, 0, halfsize * 2 * sizeof(T));
        }

        igris::array_view<T> window()
        {
            return igris::array_view<T>(dataarr + cursor, halfsize);
        }

        void cursor_fixup()
        {
            if (cursor >= halfsize)
                cursor = 0;
        }

        void _push_part(T *dt, size_t sz)
        {
            T *ptr1 = dataarr + cursor;
            T *ptr2 = dataarr + halfsize + cursor;

            std::copy(dt, dt + sz, ptr1);
            std::copy(dt, dt + sz, ptr2);

            cursor += sz;
            cursor_fixup();
        }

        size_t right_room()
        {
            return halfsize - cursor;
        }

        void push(T *dt, size_t sz)
        {
            assert(sz <= halfsize);
            size_t rroom = right_room();

            if (sz > rroom)
            {
                size_t d0_size = rroom;
                size_t d1_size = sz - rroom;

                _push_part(dt, d0_size);
                _push_part(dt + d0_size, d1_size);
            }

            else
            {
                _push_part(dt, sz);
            }
        }

        void push(const T &val)
        {
            *(dataarr + cursor) = *(dataarr + halfsize + cursor) = val;
            ++cursor;
            cursor_fixup();
        }
    };
}

#endif
