#ifndef NOS_ILIST_H
#define NOS_ILIST_H

#include <cstddef>

namespace nos
{
    template <typename T> class ilist_adapter
    {
        T &list;

    public:
        ilist_adapter(T &list) : list(list) {}

        auto begin()
        {
            return list.begin();
        }
        auto end()
        {
            return list.end();
        }
        auto begin() const
        {
            return list.begin();
        }
        auto end() const
        {
            return list.end();
        }

        auto size() const
        {
            return list.size();
        }

        const auto &operator[](size_t index) const
        {
            return list[index];
        }
    };

    template <typename T> ilist_adapter<T> ilist(T &list)
    {
        return ilist_adapter<T>(list);
    }

    template <typename T> class ibin_adapter
    {
        T &bin;

    public:
        ibin_adapter(T &bin) : bin(bin) {}

        const T &ref() const
        {
            return bin;
        }
    };

    template <typename T> ibin_adapter<T> ibin(T &bin)
    {
        return ibin_adapter<T>(bin);
    }

    template <typename T> class ibinmap_adapter
    {
        T &binmap;
        bool _print_if_one;

    public:
        ibinmap_adapter(T &binmap, bool print_if_one)
            : binmap(binmap), _print_if_one(print_if_one)
        {
        }

        const T &ref() const
        {
            return binmap;
        }

        bool print_if_one() const
        {
            return _print_if_one;
        }
    };

    template <typename T> ibinmap_adapter<T> ibinmap(T &bin, bool print_if_one)
    {
        return ibinmap_adapter<T>(bin, print_if_one);
    }

    template <typename T> class ihex_adapter
    {
        T &hex;

    public:
        ihex_adapter(T &hex) : hex(hex) {}

        const T &ref() const
        {
            return hex;
        }
    };

    template <typename T> ihex_adapter<T> ihex(T &hex)
    {
        return ihex_adapter<T>(hex);
    }

}

#endif