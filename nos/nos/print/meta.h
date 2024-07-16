#ifndef NOS_PRINT_META_H
#define NOS_PRINT_META_H

/**
    @file
*/

#include <iostream>
#include <nos/meta.h>

struct adl_finder
{
    nos::ostream &o;
    adl_finder(nos::ostream &_o) : o(_o) {}
    operator nos::ostream &()
    {
        return o;
    }
};

template <class C> class adapterbuf : public std::basic_streambuf<C>
{
private:
    typedef typename std::basic_streambuf<C>::int_type int_type;
    typedef typename std::basic_streambuf<C>::traits_type traits_type;
    using char_type = typename std::basic_streambuf<C>::char_type;

    nos::ostream &out;

public:
    adapterbuf(nos::ostream &_out) : out(_out) {}

protected:
    int_type overflow(int_type ch = traits_type::eof()) override
    {
        if (!traits_type::eq_int_type(ch, traits_type::eof()))
            out.putbyte(ch);
        return ch;
    }

    std::streamsize xsputn(const char_type *s, std::streamsize count) override
    {
        out.write(s, count);
        return count;
    }
};

namespace nos
{
    template <typename T> struct print_implementation;

    template <typename T,
              bool HasNosPrint = false,
              bool HasMtdPrint = false,
              bool HasStdOstream = false>
    struct print_implementation_solver;

    template <typename T, bool HasMtdPrint, bool HasStdOstream>
    struct print_implementation_solver<T, true, HasMtdPrint, HasStdOstream>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &os, const T &obj)
        {
            return nos_print(adl_finder(os), obj);
        }
    };

    template <typename T, bool HasStdOstream>
    struct print_implementation_solver<T, false, true, HasStdOstream>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &os, const T &obj)
        {
            return obj.print_to(os);
        }
    };

    template <typename T>
    struct print_implementation_solver<T, false, false, true>
    {
        static nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &os, const T &obj)
        {
            adapterbuf<char> adapter(os);
            std::ostream stdos(&adapter);
            stdos << obj;
            return 0;
        }
    };

    template <typename T>
    struct print_implementation
        : print_implementation_solver<
              typename std::remove_cv<T>::type,
              nos::has_nos_print<typename std::remove_cv<T>::type>::value,
              nos::has_print_method<typename std::remove_cv<T>::type>::value,
              nos::has_std_ostream<typename std::remove_cv<T>::type>::value>
    {
    };
}

#endif
