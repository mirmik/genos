#ifndef IGRIS_FLAT_SET_H
#define IGRIS_FLAT_SET_H

#include <igris/compiler.h>
#include <igris/container/flat_map.h>

namespace igris
{
    template <class Key,
              class Compare = std::less<Key>,
              class Allocator = std::allocator<Key>>
    class flat_set
    {
    private:
        std::vector<Key, Allocator> _vec = {};
        Compare _comp = {};

    public:
        using key_type = Key;
        using value_type = Key;
        using key_compare = Compare;
        using value_compare = Compare;
        using allocator_type = Allocator;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer =
            typename std::allocator_traits<Allocator>::const_pointer;
        using iterator = typename std::vector<Key, Allocator>::iterator;
        using const_iterator =
            typename std::vector<Key, Allocator>::const_iterator;
        using reverse_iterator =
            typename std::vector<Key, Allocator>::reverse_iterator;
        using const_reverse_iterator =
            typename std::vector<Key, Allocator>::const_reverse_iterator;
        using size_type = typename std::vector<Key, Allocator>::size_type;
        using difference_type =
            typename std::vector<Key, Allocator>::difference_type;

        flat_set() = default;
        flat_set(const flat_set &) = default;
        flat_set(flat_set &&) = default;
        flat_set &operator=(const flat_set &) = default;
        flat_set &operator=(flat_set &&) = default;

        explicit flat_set(const Compare &comp,
                          const Allocator &alloc = Allocator())
            : _vec(alloc)
        {
            _vec.reserve(16);
        }

        explicit flat_set(const Allocator &alloc) : _vec(alloc) {}

        allocator_type get_allocator() const
        {
            return _vec.get_allocator();
        }

        iterator begin() noexcept
        {
            return _vec.begin();
        }
        const_iterator begin() const noexcept
        {
            return _vec.begin();
        }
        const_iterator cbegin() const noexcept
        {
            return _vec.cbegin();
        }

        iterator end() noexcept
        {
            return _vec.end();
        }

        void insert(const Key &key)
        {
            auto it = std::lower_bound(_vec.begin(), _vec.end(), key, _comp);
            if (it != _vec.end() && !_comp(key, *it))
                return;
            _vec.insert(it, key);
        }

        size_t count(const Key &key) const
        {
            auto it = std::lower_bound(_vec.begin(), _vec.end(), key, _comp);
            if (it != _vec.end() && !_comp(key, *it))
                return 1;
            return 0;
        }

        void clear()
        {
            _vec.clear();
        }

        size_t size() const
        {
            return _vec.size();
        }
    };
}

#endif