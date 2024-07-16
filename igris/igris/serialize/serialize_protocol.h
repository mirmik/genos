#ifndef IGRIS_SERIALIZE_PROTOCOL_H
#define IGRIS_SERIALIZE_PROTOCOL_H

#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include <igris/serialize/serialize_tags.h>

namespace igris
{
    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    template <class T>
    concept serializer_archive = requires(T a)
    {
        a.serialize(32);
    };

    template <class T>
    concept deserializer_archive = requires(T a, uint8_t b)
    {
        {a.template deserialize<uint8_t>(b)};
        {
            a.template deserialize<uint8_t>()
            } -> std::same_as<uint8_t>;
    };

    class binary_protocol
    {
    public:
        template <serializer_archive Archive, class Container>
        static void dump(Archive &archive,
                         igris::serialize_list_tag<Container> listtag)
        {
            archive.serialize((uint16_t)listtag.size());

            auto it = listtag.container.begin();
            auto eit = listtag.container.end();
            while (it != eit)
                archive.serialize(*it++);
        }

        template <serializer_archive Archive, igris::arithmetic Type>
        static void dump(Archive &archive, const Type &obj)
        {
            archive.dump(reinterpret_cast<const char *>(&obj), sizeof(Type));
        }

        template <deserializer_archive Archive, class Container>
        static void load(Archive &archive,
                         igris::serialize_list_tag<Container> listtag)
        {
            using Type = typename Container::value_type;
            auto size = archive.template deserialize<uint16_t>();

            for (int i = 0; i < size; ++i)
            {
                Type elem = archive.template deserialize<Type>();
                listtag.container.push_back(elem);
            }
        }

        template <deserializer_archive Archive, igris::arithmetic Type>
        static void load(Archive &archive, Type &obj)
        {
            archive.load(reinterpret_cast<char *>(&obj), sizeof(Type));
        }
    };
}

#endif
