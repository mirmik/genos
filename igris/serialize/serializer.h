#ifndef IGRIS_SERIALIZE_SERIALIZER_H
#define IGRIS_SERIALIZE_SERIALIZER_H

#include <igris/serialize/serialize_checks.h>
#include <igris/serialize/serialize_protocol.h>
#include <igris/serialize/serialize_scheme.h>
#include <igris/serialize/serialize_storage.h>
#include <vector>

namespace igris
{
    template <class Storage, class Protocol = binary_protocol> class serializer
    {
        Storage &_storage;

    public:
        serializer(Storage &storage) : _storage(storage) {}

        template <igris::has_serialize_realization T>
        void serialize(const T &obj)
        {
            igris::serialize_scheme<T>::reflect(*this, obj);
        }

        template <igris::has_serialize_reflect_method T>
        void serialize(const T &obj)
        {
            obj.serialize_reflect(*this);
        }

        template <class T> void serialize(const T &obj)
        {
            Protocol::dump(*this, obj);
        }

        template <class T> void operator&(const T &obj) { serialize(obj); }

        void dump(const char *data, size_t size) { _storage.dump(data, size); }
    };

    template <class Storage, class Protocol = binary_protocol>
    class deserializer
    {
        Storage &_storage;

    public:
        deserializer(Storage &storage) : _storage(storage) {}

        template <igris::has_serialize_realization T> void deserialize(T &obj)
        {
            igris::serialize_scheme<T>::reflect(*this, obj);
        }

        template <igris::has_serialize_reflect_method T>
        void deserialize(T &obj)
        {
            obj.serialize_reflect(*this);
        }

        template <class T> void deserialize(T &obj)
        {
            Protocol::load(*this, obj);
        }

        template <class T> T deserialize()
        {
            T obj;
            deserialize(obj);
            return obj;
        }

        template <class T> void operator&(T &obj) { deserialize<T>(obj); }

        void load(char *data, size_t size) { _storage.load(data, size); }
    };
}

#endif