#ifndef IGRIS_FLAT_MAP_VIEW_H
#define IGRIS_FLAT_MAP_VIEW_H

#include <utility>
#include <cstdlib>
#include <functional>

namespace igris
{
    template <class K, class T, class KeyEqual = std::equal_to<K>>
    class flat_map_view
    {
        std::pair<K, T> *_data;
        size_t _size;
        KeyEqual equal;

    public:
        using iterator = std::pair<K, T> *;
        using const_iterator = const std::pair<K, T> *;

        iterator begin() { return _data; };
        iterator const end() { return _data + _size; };
        const_iterator begin() const { return _data; };
        const_iterator const end() const { return _data + _size; };

        size_t size() const { return _size; }

        template <unsigned int N>
        constexpr flat_map_view(std::pair<K, T> (&arr)[N])
            : _data(arr), _size(N)
        {
        }

        T &operator[](const K &key)
        {
            for (auto &p : *this)
            {
                if (equal(p.first, key))
                    return p.second;
            }
            abort();
            return *(T *)nullptr;
        }

        iterator find(const K &key)
        {
            auto it = begin();
            auto eit = end();

            for (; it != eit; ++it)
                if (equal(it->first, key))
                    break;

            return it;
        }
    };
}

#endif
