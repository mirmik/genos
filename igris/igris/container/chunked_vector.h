#ifndef IGRIS_CHUNKED_VECTOR_H
#define IGRIS_CHUNKED_VECTOR_H

/*
    Контейнер chunked_vector предназначен для запоминания потоковых
    данных в ситуации, когда размер данных неизвестен.

    Контейнер пишет данные в чанки заранее определённого размера,
    выделяя их по мере необходимости.
*/

#include <igris/container/array_view.h>
#include <igris/dprint.h>
#include <memory>

namespace igris
{
    template <class T, class Alloc1 = std::allocator<T *>,
              class Alloc2 = std::allocator<T>>
    class chunked_vector
    {
    public:
        std::vector<T *, Alloc1> _chunks = {};
        const size_t _chunk_size = 0;
        size_t _size_of_last = 0;
        Alloc2 alloc2 = {};
        T *last_chunk = nullptr;

    public:
        chunked_vector(size_t chunk_size)
            : _chunk_size(chunk_size), _size_of_last(chunk_size)
        {
        }

        chunked_vector(const chunked_vector&) =delete;
        chunked_vector& operator=(const chunked_vector&) =delete;

        size_t size() const
        {
            return _chunk_size * (_chunks.size() - 1) + _size_of_last;
        }

        void push_back(const T &obj)
        {
            if (_chunk_size == _size_of_last)
            {
                allocate_next_chunk();
            }

            *(last_chunk + _size_of_last) = obj;
            _size_of_last++;
        }

        T &operator[](size_t n)
        {
            size_t chunk = n / _chunk_size;
            size_t idx = n % _chunk_size;
            return _chunks[chunk][idx];
        }

        const T &operator[](size_t n) const
        {
            size_t chunk = n / _chunk_size;
            size_t idx = n % _chunk_size;
            return _chunks[chunk][idx];
        }

        void allocate_next_chunk()
        {
            last_chunk = alloc2.allocate(_chunk_size);
            _chunks.push_back(last_chunk);
            _size_of_last = 0;
        }

        igris::array_view<T> chunk(size_t n)
        {
            if (n == _chunks.size() - 1)
                return {_chunks[n], last_chunk};
            return {_chunks[n], _chunk_size};
        }

        size_t chunks_count() { return _chunks.size(); }

        std::vector<T> to_vector()
        {
            std::vector<T> ret;
            ret.resize(size());

            auto dit = ret.begin();
            for (unsigned int i = 0; i < _chunks.size() - 1; ++i)
            {
                igris::array_view<T> ch{_chunks[i], _chunk_size};
                std::copy(ch.begin(), ch.end(), dit);
                std::advance(dit, _chunk_size);
            }

            igris::array_view<T> ch{_chunks[_chunks.size() - 1], _size_of_last};
            std::copy(ch.begin(), ch.end(), dit);

            return ret;
        }

        ~chunked_vector()
        {
            for (auto *ptr : _chunks)
            {
                alloc2.deallocate(ptr, _chunk_size);
            }
        }
    };
}

#endif
