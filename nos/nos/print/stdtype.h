#ifndef NOS_PRINT_STDTYPE_H
#define NOS_PRINT_STDTYPE_H

#include <nos/ilist.h>
#include <nos/util/buffer.h>
#include <stdint.h>
#include <stdlib.h>

#if __has_include(<complex>)
#include <complex>
#endif

#include <string>
#include <string_view>

namespace nos
{
    class ostream;
}

int nos_print(nos::ostream &out, const char *str);
int nos_print(nos::ostream &out, char *str);
int nos_print(nos::ostream &out, bool str);

int nos_print(nos::ostream &out, int8_t str);
int nos_print(nos::ostream &out, int16_t str);
int nos_print(nos::ostream &out, int32_t str);
int nos_print(nos::ostream &out, int64_t str);

int nos_print(nos::ostream &out, uint8_t str);
int nos_print(nos::ostream &out, uint16_t str);
int nos_print(nos::ostream &out, uint32_t str);
int nos_print(nos::ostream &out, uint64_t str);

int nos_print(nos::ostream &out, float str);
int nos_print(nos::ostream &out, double str);
int nos_print(nos::ostream &out, long double str);

int nos_print(nos::ostream &out, const nos::buffer &buf);

#include <nos/print.h>

#if __has_include(<vector>)
#include <vector>
#endif

#if __has_include(<array>)
#include <array>
#endif

#if __has_include(<list>)
#include <list>
#endif

namespace nos
{
    template <> struct print_implementation<const char *>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const char *const obj)
        {
            return nos_print(out, obj);
        }
    };

    template <> struct print_implementation<char *>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, char *const obj)
        {
            return nos_print(out, obj);
        }
    };

    template <typename T> struct print_implementation<T *>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const T *obj)
        {
            return *nos::printptr_to(out, obj);
        }
    };

#if __has_include(<vector>)
    template <typename T> struct print_implementation<std::vector<T>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const std::vector<T> &obj)
        {
            return *nos::print_list_to<std::vector<T>>(out, obj);
        }
    };
#endif

    template <typename CharT>
    struct print_implementation<std::basic_string<CharT>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const std::basic_string<CharT> &obj)
        {
            return *nos::write_to(out, obj.data(), obj.size());
        }
    };

    template <typename CharT>
    struct print_implementation<std::basic_string_view<CharT>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const std::basic_string_view<CharT> &obj)
        {
            return *nos::write_to(out, obj.data(), obj.size());
        }
    };

#if __has_include(<list>)
    template <typename T> struct print_implementation<std::list<T>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const std::list<T> &obj)
        {
            return *nos::print_list_to<std::list<T>>(out, obj);
        }
    };
#endif

    template <class T0, class T1> struct print_implementation<std::pair<T0, T1>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const std::pair<T0, T1> &obj)
        {
            size_t res = 0;
            res += *nos::print_to(out, "{");
            res += *nos::print_to(out, obj.first);
            res += *nos::print_to(out, ",");
            res += *nos::print_to(out, obj.second);
            res += *nos::print_to(out, "}");
            return res;
        }
    };

#if __has_include(<array>)
    template <typename T, size_t M>
    struct print_implementation<std::array<T, M>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const std::array<T, M> &obj)
        {
            return nos::print_list_to<std::array<T, M>>(out, obj);
        }
    };
#endif

#if __has_include(<complex>)
    template <typename T> struct print_implementation<std::complex<T>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const std::complex<T> &obj)
        {
            size_t ret = 0;

            T re = obj.real();
            T im = obj.imag();

            ret += *nos::print_to(out, re);
            ret += *nos::print_to(out, im < 0 ? "-" : "+");
            ret += *nos::print_to(out, ABS(im));
            ret += *nos::print_to(out, "j");
            return ret;
        }
    };
#endif

    template <typename T> struct print_implementation<nos::ilist_adapter<T>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const nos::ilist_adapter<T> &obj)
        {
            size_t ret = 0;
            ret += *nos::print_to(out, "[");
            for (size_t i = 0; i < obj.size() - 1; i++)
            {
                ret += *nos::print_to(out, obj[i]);
                if (i != obj.size() - 1)
                {
                    ret += *nos::print_to(out, ",");
                }
            }
            ret += *nos::print_to(out, obj[obj.size() - 1]);
            ret += *nos::print_to(out, "]");
            return ret;
        }
    };

    template <typename T> struct print_implementation<nos::ibin_adapter<T>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const nos::ibin_adapter<T> &obj)
        {
            size_t ret = 0;
            ret += *nos::print_to(out, "0b");
            ret += *nos::printbin_to(out, &obj.ref(), sizeof(obj.ref()));
            return ret;
        }
    };

    template <typename T> struct print_implementation<nos::ibinmap_adapter<T>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const nos::ibinmap_adapter<T> &obj)
        {
            size_t ret = 0;
            auto bytesize = sizeof(obj.ref());
            ret += *nos::print_to(out, "{");
            for (int i = (int)bytesize - 1; i >= 0; i--)
            {
                uint8_t *ptr = (uint8_t *)(&obj.ref()) + i;
                for (int j = 7; j >= 0; j--)
                {
                    int bitno = i * 8 + j;
                    int bitval = *ptr & (1 << j);

                    bool print_if_one = obj.print_if_one();

                    if ((bool)bitval == print_if_one)
                        ret += *nos::print_to(
                            out,
                            std::to_string(bitno) + std::string(":") +
                                std::string(bitval ? "1" : "0") + ",");
                }
            }
            ret += *nos::print_to(out, "}");
            return ret;
        }
    };

    template <typename T> struct print_implementation<nos::ihex_adapter<T>>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &out, const nos::ihex_adapter<T> &obj)
        {
            size_t ret = 0;
            ret += *nos::print_to(out, "0x");
            ret += *nos::printhex_to(
                out, (const void *)&obj.ref(), (size_t)sizeof(obj.ref()));
            return ret;
        }
    };

}

#endif
