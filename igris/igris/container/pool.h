#ifndef IGRIS_POOL_H
#define IGRIS_POOL_H

#include <igris/datastruct/pool.h>

namespace igris
{
    class pool
    {
    private:
        struct pool_head head = POOL_HEAD_INIT(head);

        void *_zone=nullptr;
        size_t _size=0;
        size_t _elemsz=0;

        int _count=0;

    public:
        size_t size() const { return _size / _elemsz; }

        size_t room() const { return _count; }

        bool cell_is_allocated(int i)
        {
            if (i >= (int)size() || i < 0)
                return false;
            void *ptr = (void *)((char *)_zone + i * _elemsz);
            return !pool_in_freelist(&head, ptr);
        }

        void *cell(int i) { return (void *)((char *)_zone + _elemsz * i); }

        class unlinked_iterator
        {
        public:
            pool *_pool;
            int _num;

        public:
            unlinked_iterator(const pool *_pool, int num)
                : _pool((pool *)_pool), _num(num)
            {
            }

            unlinked_iterator next()
            {
                do
                {
                    ++_num;
                } while (_num < (int)_pool->size() &&
                         !_pool->cell_is_allocated(_num));

                return {_pool, _num < (int)_pool->size() ? _num : -1};
            }

            bool operator==(const unlinked_iterator &oth)
            {
                return oth._pool == _pool && oth._num == _num;
            }

            bool operator!=(const unlinked_iterator &oth)
            {
                return oth._pool != _pool || oth._num != _num;
            }

            unlinked_iterator& operator++()
            {
                *this = next();
                return *this;
            }
            void *operator*() { return _pool->cell(_num); }
        };

        unlinked_iterator begin() const
        {
            return unlinked_iterator{this, -1}.next();
        };
        const unlinked_iterator end() const { return {this, -1}; }

        pool() = default;

        pool(void *zone, size_t size, size_t elsize)
        {
            init(zone, size, elsize);
        }

        void init(void *zone, size_t size, size_t elsize)
        {
            _zone = zone;
            _size = size;
            _elemsz = elsize;
            pool_init(&head);
            pool_engage(&head, zone, size, elsize);
            _count = this->size();
        }

        void *get()
        {
            void *ret = pool_alloc(&head);
            _count--;
            return ret;
        }

        void put(void *ptr)
        {
            if (ptr == NULL)
                return;

            assert((uintptr_t)ptr >= (uintptr_t)_zone);
            assert((uintptr_t)ptr < (uintptr_t)_zone + _size);

            pool_free(&head, ptr);
            _count++;
        }

        size_t element_size() const { return _elemsz; }

        size_t avail() const { return pool_avail(&head); }

        void debug_print()
        {
            DPRINTPTR(_zone);
            DPRINT(_size);
            DPRINT(_elemsz);
            debug_print_pool_head(&head);
        }
    };
}

#endif
