#ifndef IGRIS_SERIALIZE_ARCHIEVE_H
#define IGRIS_SERIALIZE_ARCHIEVE_H

#include <igris/serialize/serializer.h>

#include <igris/serialize/serialize_protocol.h>
#include <igris/serialize/serialize_scheme.h>
#include <igris/serialize/serialize_storage.h>
#include <igris/serialize/serialize_tags.h>

static_assert(igris::has_serialize_realization<std::vector<int>>);

class Test
{
    int a, b, c;

public:
    template <class Archive> void serialize_reflect(Archive &arch)
    {
        arch &a;
        arch &b;
        arch &c;
    }
};

static_assert(igris::has_serialize_reflect_method<Test>);

namespace igris
{
    template <class Protocol = binary_protocol, class T, class Storage>
    void serialize(const T &obj, Storage &storage)
    {
        igris::serializer<Storage, Protocol> archive(storage);
        archive.serialize(obj);
    }

    template <class Protocol = binary_protocol, class T>
    std::string serialize(const T &obj)
    {
        string_storage storage;
        igris::serializer<string_storage, Protocol> archive(storage);
        archive.serialize(obj);
        return storage.storage();
    }

    template <class T, class Protocol = binary_protocol>
    T deserialize(const std::string &str)
    {
        deserialize_buffer_storage storage(str);
        igris::deserializer<deserialize_buffer_storage, Protocol> archive(
            storage);
        return archive.template deserialize<T>();
    }

    template <class T, load_storage_type Storage,
              class Protocol = binary_protocol>
    T deserialize(Storage &storage)
    {
        igris::deserializer<Storage, Protocol> archive(storage);
        return archive.template deserialize<T>();
    }

}

#endif