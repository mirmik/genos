#ifndef NOS_SERIALIZE_H
#define NOS_SERIALIZE_H

#include <cstdint>
#include <nos/binary/bytearray.h>
#include <nos/io/bytearray_ostream.h>
#include <nos/io/ostream.h>
#include <string>
#include <tuple>

#define SERIALIZE_HELPER_NUMERIC(T)                                            \
    template <> struct serialize_helper<T>                                     \
    {                                                                          \
        static void serialize_to(nos::ostream &os, T obj)                      \
        {                                                                      \
            os.write(&obj, sizeof(T));                                         \
        }                                                                      \
    }

namespace nos
{
    template <class T> void serialize_to(nos::ostream &os, const T &obj);

    class SerializeReflector
    {
    public:
        nos::ostream &os;

    public:
        SerializeReflector(nos::ostream &os) : os(os) {}

        template <class T> void operator()(const T &obj)
        {
            nos::serialize_to(os, obj);
        }
    };

    template <class T> class serialize_helper
    {
    public:
        static void serialize_to(nos::ostream &os, const T &obj)
        {
            nos::SerializeReflector op(os);
            const_cast<T &>(obj).reflect(op);
        }
    };

    SERIALIZE_HELPER_NUMERIC(char);
    SERIALIZE_HELPER_NUMERIC(signed char);
    SERIALIZE_HELPER_NUMERIC(signed short);
    SERIALIZE_HELPER_NUMERIC(signed int);
    SERIALIZE_HELPER_NUMERIC(signed long);
    SERIALIZE_HELPER_NUMERIC(signed long long);
    SERIALIZE_HELPER_NUMERIC(unsigned char);
    SERIALIZE_HELPER_NUMERIC(unsigned short);
    SERIALIZE_HELPER_NUMERIC(unsigned int);
    SERIALIZE_HELPER_NUMERIC(unsigned long);
    SERIALIZE_HELPER_NUMERIC(unsigned long long);

    // TODO: что делать с размерами типов float и double?
    template <> struct serialize_helper<float>
    {
        static void serialize_to(nos::ostream &os, const float &obj)
        {
            os.write(&obj, sizeof(float));
        }
    };
    template <> struct serialize_helper<double>
    {
        static void serialize_to(nos::ostream &os, const double &obj)
        {
            os.write(&obj, sizeof(double));
        }
    };

    template <> struct serialize_helper<std::string>
    {
        static void serialize_to(nos::ostream &os, const std::string &obj)
        {
            uint64_t size = obj.size();
            if (size < ((uint64_t)1 << 8))
            {
                os.put(1);
                nos::serialize_to<uint8_t>(os, static_cast<uint8_t>(size));
            }
            else if (size < ((uint64_t)1 << 16))
            {
                os.put(2);
                nos::serialize_to<uint16_t>(os, static_cast<uint16_t>(size));
            }
            else if (size < ((uint64_t)1 << 32))
            {
                os.put(4);
                nos::serialize_to<uint32_t>(os, static_cast<uint32_t>(size));
            }
            else
            {
                os.put(8);
                nos::serialize_to<uint64_t>(os, static_cast<uint64_t>(size));
            }
            os.write(obj.data(), size);
        }
    };

    template <typename... Args> struct serialize_helper<std::tuple<Args...>>
    {
        using Tuple = std::tuple<Args...>;

        template <typename std::size_t... I>
        static void serialize_to(nos::ostream &keeper,
                                 const Tuple &tpl,
                                 std::index_sequence<I...>)
        {
            int ___[] = {(nos::serialize_to(keeper, std::get<I>(tpl)), 0)...};
            (void)___;
        }

        static void serialize_to(nos::ostream &keeper, const Tuple &tpl)
        {
            serialize_to(keeper, tpl, std::index_sequence_for<Args...>{});
        }
    };

    template <class T> void serialize_to(nos::ostream &os, const T &obj)
    {
        serialize_helper<T>::serialize_to(os, obj);
    }

    template <class T> nos::bytearray serialize(const T &obj)
    {
        nos::bytearray array;
        nos::bytearray_ostream writer(array);
        serialize_to(writer, obj);
        return array;
    }
}

#endif