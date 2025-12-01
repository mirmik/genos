#ifndef IGRIS_SERIALIZE_SERIALIZE_SCHEME_H__
#define IGRIS_SERIALIZE_SERIALIZE_SCHEME_H__

#include <igris/serialize/serialize_tags.h>

namespace igris
{
    template <class T> struct serialize_scheme
    {
    };

    template <class T> struct serialize_scheme<std::vector<T>>
    {
        template <class Archive>
        static void reflect(Archive &archive, const std::vector<T> &vec)
        {
            auto listtag = igris::serialize_list_tag(vec);
            archive &listtag;
        }
    };
}

#endif