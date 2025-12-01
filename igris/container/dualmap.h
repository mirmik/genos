#ifndef IGRIS_CONTAINER_DUALMAP_H
#define IGRIS_CONTAINER_DUALMAP_H

#include <map>

namespace igris
{
    template <class K1, class K2, class T> class dualmap
    {
    private:
        struct record;

    public:
        using origin_t = std::map<std::pair<K1, K2>, record>;
        using iter0 = typename origin_t::iterator;

        using hash1_t = std::multimap<K1, iter0>;
        using hash2_t = std::multimap<K2, iter0>;
        using iter1 = typename hash1_t::iterator;
        using iter2 = typename hash2_t::iterator;

        using Key = std::pair<K1, K2>;

    private:
        origin_t origin;
        hash1_t hash1;
        hash2_t hash2;

        struct record
        {
            iter1 it1;
            iter2 it2;
            T val;
        };

    public:
        std::tuple<iter0, iter1, iter2, bool> insert(const K1 &k1, const K2 &k2,
                                                     const T &val)
        {
            iter0 it0;
            iter1 it1;
            iter2 it2;

            std::pair<iter0, bool> ret0;

            record rec = {{}, {}, val};

            ret0 = origin.insert(make_pair(std::make_pair(k1, k2), rec));

            if (ret0.second == false)
                return std::make_tuple(it0, it1, it2, false);

            it0 = ret0.first;

            it1 = hash1.insert(std::make_pair(k1, it0));
            it2 = hash2.insert(std::make_pair(k2, it0));

            it0->second.it1 = it1;
            it0->second.it2 = it2;

            return std::make_tuple(it0, it1, it2, true);
        }

        T &at(const std::pair<K1, K2> &key) { return origin.at(key).val; }

        std::pair<iter1, iter1> equal_range1(const K1 &k1)
        {
            return hash1.equal_range(k1);
        }

        std::pair<iter2, iter2> equal_range2(const K2 &k2)
        {
            return hash2.equal_range(k2);
        }

        iter0 find(const Key &k) { return origin.find(k); }

        bool remove(const Key &k)
        {
            iter0 it0 = origin.find(k);

            if (it0 == origin.end())
                return false;

            return erase(it0);
        }

        bool erase(iter0 it0)
        {
            hash1.erase(it0->second.it1);
            hash2.erase(it0->second.it2);
            origin.erase(it0);

            return true;
        }

        size_t count(const Key &k) { return origin.count(k); }

        size_t count1(const K1 &k1) { return hash1.count(k1); }

        size_t count2(const K2 &k2) { return hash2.count(k2); }

        bool contains(const Key &k) { return origin.count(k); }

        auto begin0() { return origin.begin(); }
        auto end0() { return origin.end(); }

        auto begin1() { return hash1.begin(); }
        auto end1() { return hash1.end(); }

        auto begin2() { return hash2.begin(); }
        auto end2() { return hash2.end(); }

        int size() { return origin.size(); }
    };
}

#endif
