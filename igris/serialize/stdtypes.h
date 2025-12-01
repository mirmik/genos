#ifndef IGRIS_SERIALIZE_STDTYPES_H
#define IGRIS_SERIALIZE_STDTYPES_H

/**
@file
*/

#include <igris/serialize/serialize.h>

#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace igris
{
    template <typename Archive, typename... Args>
    struct serialize_helper<Archive, std::tuple<Args...>>
    {
        using Tuple = std::tuple<Args...>;

        template <typename std::size_t... I>
        static void tuple_serialize_helper(Archive &keeper, const Tuple &tpl,
                                           std::index_sequence<I...>)
        {
            int ___[] = {(igris::serialize(keeper, std::get<I>(tpl)), 0)...};
        }

        static void serialize(Archive &keeper, const Tuple &tpl)
        {
            tuple_serialize_helper(keeper, tpl,
                                   std::index_sequence_for<Args...>{});
        }

        template <typename std::size_t... I>
        static void tuple_deserialize_helper(Archive &keeper, Tuple &tpl,
                                             std::index_sequence<I...>)
        {
            int ___[] = {(igris::deserialize(keeper, std::get<I>(tpl)), 0)...};
        }

        static void deserialize(Archive &keeper, Tuple &tpl)
        {
            tuple_deserialize_helper(keeper, tpl,
                                     std::index_sequence_for<Args...>{});
        }
    };

    template <typename Archive> struct serialize_helper<Archive, std::string>
    {
        static void serialize(Archive &keeper, const std::string &str)
        {
            igris::serialize(keeper, igris::buffer(str.data(), str.size()));
        }

        static void deserialize(Archive &keeper, std::string &str)
        {
            uint16_t size;
            igris::deserialize(keeper, size);
            str.resize(size);
            keeper.load_data((char *)str.data(), str.size());
        }
    };

    template <typename Archive, class F, class S>
    struct serialize_helper<Archive, std::pair<F, S>>
    {
        static void serialize(Archive &keeper, const std::pair<F, S> &pair)
        {
            igris::serialize(keeper, pair.first);
            igris::serialize(keeper, pair.second);
        }

        static void deserialize(Archive &keeper, std::pair<F, S> &pair)
        {
            igris::deserialize(keeper, pair.first);
            igris::deserialize(keeper, pair.second);
        }
    };

    template <typename Archive, typename T>
    struct serialize_helper<Archive, std::vector<T>>
    {
        static void serialize(Archive &keeper, const std::vector<T> &vec)
        {
            igris::serialize(keeper, (uint16_t)vec.size());
            igris::serialize(keeper,
                             igris::archive::data<T>{vec.data(), vec.size()});
        }

        static void deserialize(Archive &keeper, std::vector<T> &vec)
        {
            uint16_t size;
            igris::deserialize(keeper, size);

            for (int i = 0; i < size; i++)
            {
                T value;
                igris::deserialize(keeper, value);
                vec.push_back(value);
            }
        }
    };

    template <typename Archive, class K, class T>
    struct serialize_helper<Archive, std::map<K, T>>
    {
        static void serialize(Archive &keeper, const std::map<K, T> &map)
        {
            igris::serialize(keeper, (uint16_t)map.size());
            // igris::serialize(keeper, igris::archive::data<T> {vec.data(),
            // vec.size()});
            for (auto pair : map)
            {
                igris::serialize(keeper, pair);
            }
        }

        static void deserialize(Archive &keeper, std::map<K, T> &map)
        {
            uint16_t size;
            igris::deserialize(keeper, size);

            for (int i = 0; i < size; i++)
            {
                // typename std::map<K,T>::value_type pair;
                K first;
                T second;
                igris::deserialize(keeper, first);
                igris::deserialize(keeper, second);
                map.insert(std::make_pair(first, second));
            }
        }
    };

    template <class T> std::string serialize(const T &obj)
    {
        std::string ret;
        igris::archive::binary_string_writer writer(ret);

        igris::serialize(writer, obj);

        return ret;
    }

    template <class T> T deserialize(const igris::buffer &in)
    {
        T ret;

        igris::archive::binary_buffer_reader reader(in.data(), in.size());
        igris::deserialize(reader, ret);

        return ret;
    }

    template <class T> T deserialize(const std::string &in)
    {
        return deserialize<T>(igris::buffer(in.data(), in.size()));
    }
}

#endif
