#ifndef IGRIS_CONTAINER_FLAT_MAP_H
#define IGRIS_CONTAINER_FLAT_MAP_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace igris
{
    template <class Key,
              class T,
              class Compare = std::less<Key>,
              class Alloc = std::allocator<std::pair<Key, T>>>
    class flat_map
    {
    public:
        using value_type = std::pair<Key, T>;
        using iterator = typename std::vector<value_type>::iterator;
        using const_iterator = typename std::vector<value_type>::const_iterator;
        using reverse_iterator =
            typename std::vector<value_type>::reverse_iterator;
        using const_reverse_iterator =
            typename std::vector<value_type>::const_reverse_iterator;
        using size_type = typename std::vector<value_type>::size_type;
        using difference_type =
            typename std::vector<value_type>::difference_type;
        using mapped_type = T;
        using reference = value_type &;
        using const_reference = const value_type &;
        using key_type = Key;

    private:
        std::vector<value_type, Alloc> storage = {};

    public:
        flat_map() = default;
        flat_map(const flat_map &) = default;
        flat_map(flat_map &&) = default;
        flat_map &operator=(const flat_map &) = default;
        flat_map &operator=(flat_map &&) = default;

        flat_map(const std::initializer_list<value_type> &init) : storage(init)
        {
        }

        bool operator==(const flat_map &other) const
        {
            return storage == other.storage;
        }

        bool operator!=(const flat_map &other) const
        {
            return storage != other.storage;
        }

        iterator begin()
        {
            return storage.begin();
        }
        const_iterator begin() const
        {
            return storage.begin();
        }
        iterator end()
        {
            return storage.end();
        }
        const_iterator end() const
        {
            return storage.end();
        }
        reverse_iterator rbegin()
        {
            return storage.rbegin();
        }
        const_reverse_iterator rbegin() const
        {
            return storage.rbegin();
        }
        reverse_iterator rend()
        {
            return storage.rend();
        }
        const_reverse_iterator rend() const
        {
            return storage.rend();
        }
        const_iterator cbegin() const
        {
            return storage.cbegin();
        }
        const_iterator cend() const
        {
            return storage.cend();
        }
        const_reverse_iterator crbegin() const
        {
            return storage.crbegin();
        }
        const_reverse_iterator crend() const
        {
            return storage.crend();
        }
        size_type size() const
        {
            return storage.size();
        }
        bool empty() const
        {
            return storage.empty();
        }
        size_type max_size() const
        {
            return storage.max_size();
        }
        void reserve(size_type n)
        {
            storage.reserve(n);
        }
        size_type capacity() const
        {
            return storage.capacity();
        }
        void shrink_to_fit()
        {
            storage.shrink_to_fit();
        }
        void clear()
        {
            storage.clear();
        }
        void swap(flat_map &other)
        {
            storage.swap(other.storage);
        }

        T &operator[](const Key &key)
        {
            auto it = std::find_if(
                storage.begin(), storage.end(), [&key](const value_type &p) {
                    return p.first == key;
                });

            if (it == storage.end())
            {
                storage.push_back(value_type(key, T()));
                return storage.back().second;
            }

            return it->second;
        }

        const T &operator[](const Key &key) const
        {
            auto it = std::find_if(
                storage.begin(), storage.end(), [&key](const value_type &p) {
                    return p.first == key;
                });

            if (it == storage.end())
            {
                return T();
            }

            return it->second;
        }

        T &at(const Key &key)
        {
            auto it = std::find_if(
                storage.begin(), storage.end(), [&key](const value_type &p) {
                    return p.first == key;
                });

            if (it == storage.end())
            {
                throw std::out_of_range("flat_map::at");
            }

            return it->second;
        }

        const T &at(const Key &key) const
        {
            auto it = std::find_if(
                storage.begin(), storage.end(), [&key](const value_type &p) {
                    return p.first == key;
                });

            if (it == storage.end())
            {
                throw std::out_of_range("flat_map::at");
            }

            return it->second;
        }

        iterator find(const Key &key)
        {
            return std::find_if(
                storage.begin(), storage.end(), [&key](const value_type &p) {
                    return p.first == key;
                });
        }

        const_iterator find(const Key &key) const
        {
            return std::find_if(
                storage.begin(), storage.end(), [&key](const value_type &p) {
                    return p.first == key;
                });
        }

        size_type count(const Key &key) const
        {
            return std::count_if(
                storage.begin(), storage.end(), [&key](const value_type &p) {
                    return p.first == key;
                });
        }

        template <class... Args>
        std::pair<iterator, bool> emplace(Key key, Args &&... args)
        {
            auto it = std::find_if(
                storage.begin(),
                (iterator)storage.end(),
                [&key](const value_type &p) { return p.first == key; });
            if (it != storage.end())
            {
                return std::make_pair(it, false);
            }
            storage.push_back(std::pair(key, T(std::forward<Args>(args)...)));
            return std::make_pair(storage.end() - 1, true);
        }

        iterator insert(const value_type &value)
        {
            auto it = std::find_if(storage.begin(),
                                   (iterator)storage.end(),
                                   [&value](const value_type &p) {
                                       return p.first == value.first;
                                   });
            if (it != storage.end())
            {
                return it;
            }
            return storage.insert(
                std::upper_bound(storage.begin(),
                                 (iterator)storage.end(),
                                 value,
                                 [](const value_type &a, const value_type &b) {
                                     return a.first < b.first;
                                 }),
                value);
        }
    };
}

#endif