#ifndef GXX_STD_PAIR
#define GXX_STD_PAIR
#include "../igris_std_config.hpp"

namespace IGRIS_STD_NS
{
    template <typename T1, typename T2> class pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

    public:
        first_type first;
        second_type second;
        constexpr pair(){};
        pair(first_type _first, second_type _second)
            : first(_first), second(_second){};
        pair(const pair &p) : first(p.first), second(p.second){};
        pair(pair &&p) : first(p.first), second(p.second){};

        pair &operator=(const pair &b)
        {
            first = b.first;
            second = b.second;
            return *this;
        }

        pair &operator=(pair &&b)
        {
            first = b.first;
            second = b.second;
            return *this;
        }
    };

    template <class T1, class T2>
    pair<T1, T2> make_pair(const T1 &x, const T2 &y)
    {
        return (pair<T1, T2>(x, y));
    }

    template <class T1, class T2>
    constexpr bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class T1, class T2>
    constexpr bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return lhs.first != rhs.first || lhs.second != rhs.second;
    }

    template <class T1, class T2>
    bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        if (lhs.first < rhs.first)
            return true;
        if (lhs.second < rhs.second)
            return true;
        return false;
    }
}

#endif
