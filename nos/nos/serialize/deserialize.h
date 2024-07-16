#ifndef NOS_DESERIALIZE_H
#define NOS_DESERIALIZE_H

#include <nos/binary/bytearray.h>
#include <nos/io/istream.h>
#include <nos/io/bytearray_istream.h>
#include <cstdint>
#include <string>
#include <tuple>

#define DESERIALIZE_HELPER_NUMERIC(T)                           \
template <> struct deserialize_helper<T>                        \
{                                                               \
    static void deserialize_from(nos::istream& os, T& obj)      \
    {                                                           \
        os.read(&obj, sizeof(T));                               \
    }                                                           \
}

namespace nos
{
    template <class T> void deserialize_from(nos::istream& os, T& obj);
    template <class T> T deserialize_from(nos::istream& os);
        
    class DeserializeReflector 
    {
    public:
        nos::istream& os;
    public:
        DeserializeReflector(nos::istream& os) : os(os) {}

        template <class T>
        void operator()(T& obj) 
        {
            nos::deserialize_from(os, obj);
        }
    };

    template <class T> class deserialize_helper 
    {
    public:
        static void deserialize_from(nos::istream& os, T& obj) 
        { 
            nos::DeserializeReflector op(os);
            obj.reflect(op);
        }
    };

    DESERIALIZE_HELPER_NUMERIC(char);
    DESERIALIZE_HELPER_NUMERIC(signed char);
    DESERIALIZE_HELPER_NUMERIC(signed short);
    DESERIALIZE_HELPER_NUMERIC(signed int);
    DESERIALIZE_HELPER_NUMERIC(signed long);
    DESERIALIZE_HELPER_NUMERIC(signed long long);
    DESERIALIZE_HELPER_NUMERIC(unsigned char);
    DESERIALIZE_HELPER_NUMERIC(unsigned short);
    DESERIALIZE_HELPER_NUMERIC(unsigned int);
    DESERIALIZE_HELPER_NUMERIC(unsigned long);
    DESERIALIZE_HELPER_NUMERIC(unsigned long long);
    template <> struct deserialize_helper<float > { static void deserialize_from(nos::istream& os, float & obj) { os.read(&obj, sizeof(float )); } };
    template <> struct deserialize_helper<double> { static void deserialize_from(nos::istream& os, double& obj) { os.read(&obj, sizeof(double)); } };

    template <> struct deserialize_helper<std::string> { 
        static void deserialize_from(nos::istream& os, std::string& obj) 
        {
            size_t size; 
            uint8_t sizesize = nos::deserialize_from<uint8_t>(os);
                 if (sizesize == 1) { size = nos::deserialize_from<uint8_t >(os); }
            else if (sizesize == 2) { size = nos::deserialize_from<uint16_t>(os); }
            else if (sizesize == 4) { size = nos::deserialize_from<uint32_t>(os); }
            else                    { size = nos::deserialize_from<uint64_t>(os); }
            obj.resize(size);
            os.read(&obj[0], size);
        } 
    };

    template <typename... Args>
    struct deserialize_helper<std::tuple<Args...>>
    {
        using Tuple = std::tuple<Args...>;

        template <typename std::size_t... I>
        static void deserialize_from(
            nos::istream& keeper, 
            Tuple &tpl,
            std::index_sequence<I...>)
        {
            int ___[] = {(nos::deserialize_from<
                std::remove_reference_t<decltype(std::get<I>(tpl))>>
                    (keeper, std::get<I>(tpl)), 0)...};
            (void) ___;
        }

        static void deserialize_from(nos::istream& keeper, Tuple &tpl)
        {
            deserialize_from(keeper, tpl, std::index_sequence_for<Args...>{});
        }
    };

    template <class T> 
    void deserialize_from(nos::istream& os, T& obj) 
    {
        deserialize_helper<T>::deserialize_from(os, obj);
    }

    template <class T> 
    T deserialize_from(nos::istream& os) 
    {
        T obj = {};
        deserialize_helper<T>::deserialize_from(os, obj);
        return obj;
    }
    
    template <class T> 
    T deserialize(const nos::bytearray& array) 
    {
        nos::bytearray_istream reader(array);
        return nos::deserialize_from<T>(reader);
    }
}

#endif