#ifndef IGRIS_PROTOCOLS_INVOKE_SERIALIZER_H
#define IGRIS_PROTOCOLS_INVOKE_SERIALIZER_H

#include <igris/buffer.h>
#include <igris/serialize/serialize.h>

namespace igris
{
    /*	template <class Archive, class ... Args>
        int serialize_args(Archive& archieve, Args&& ... args)
        {
            const std::tuple<const Args&...> targs(std::forward<Args>(args)
       ...); igris::serialize(archieve, targs);
        }

        template <class Archive, class ... Args>
        int serialize_invoke(Archive& archieve, igris::buffer header, Args&& ...
       args)
        {
            int acc;
            igris::result<int> ret;
            (ret=igris::serialize(archieve, header)) ? return : acc+=ret.value;
            igris::serialize_args(archieve, std::forward<Args>(args) ...);
        }*/
}

#endif