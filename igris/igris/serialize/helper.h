#ifndef IGRIS_SERIALIZE_HELPER_H
#define IGRIS_SERIALIZE_HELPER_H

/**
@file
*/

#include <igris/util/signature.h>

IGRIS_SIGNATURE_ATTRIBUTE_CHECKER(igris_has_serialize, serialize)

namespace igris
{
    template <typename M, typename T, bool HaveSerialize = true>
    struct serialize_helper_basic
    {
        static void serialize(M &keeper, const T &obj)
        {
            obj.serialize(keeper);
        }

        static void deserialize(M &keeper, T &obj) { obj.deserialize(keeper); }
    };

    template <typename M, typename T> struct serialize_helper_basic<M, T, false>
    {
        static void serialize(M &keeper, const T &obj) { keeper.dump(obj); }

        static void deserialize(M &keeper, T &obj) { keeper.load(obj); }
    };

    template <typename M, typename T>
    struct serialize_helper
        : public serialize_helper_basic<M, T, igris_has_serialize<T>()>
    {
    };

    template <typename M, typename T>
    inline void serialize(M &keeper, const T &obj)
    {
        serialize_helper<M, T>::serialize(keeper, obj);
    }

    template <typename M, typename T> inline void deserialize(M &keeper, T &obj)
    {
        serialize_helper<M, std::remove_cv_t<T>>::deserialize(keeper, obj);
    }

    template <typename M, typename T>
    inline void deserialize(M &keeper, T &&obj)
    {
        serialize_helper<M, std::remove_cv_t<T>>::deserialize(keeper,
                                                              std::move(obj));
    }
}

#endif
