#ifndef IGRIS_SERIALIZE_SERIALIZE_CHECKS_H
#define IGRIS_SERIALIZE_SERIALIZE_CHECKS_H

#include <igris/serialize/serialize_protocol.h>
#include <igris/serialize/serialize_scheme.h>
#include <igris/serialize/serialize_storage.h>

namespace igris
{
    template <class A, class B> class serializer;
    using stub_archive =
        igris::serializer<igris::appendable_storage<std::string>,
                          binary_protocol>;

    template <class T, class Archive = igris::stub_archive>
    concept has_serialize_realization = requires(T obj, Archive a)
    {
        igris::serialize_scheme<T>::reflect(a, obj);
    };

    template <class T, class Archive = igris::stub_archive>
    concept has_serialize_reflect_method = requires(T obj, Archive a)
    {
        obj.serialize_reflect(a);
    };
}

#endif