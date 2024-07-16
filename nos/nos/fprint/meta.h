#ifndef NOS_FPRINT_META_H
#define NOS_FPRINT_META_H

/**
    @file
*/

#include <nos/io/ostream.h>
#include <nos/meta.h>
#include <nos/print/meta.h>
#include <nos/print/stdtype.h>
#include <nos/util/buffer.h>

namespace nos
{
    template <typename T> struct fprint_implementation;

    template <typename T, bool HasNosFPrint = false, bool HasMtdFPrint = false>
    struct fprint_implementation_solver
    {
        static nos::expected<size_t, nos::output_error>
        fprint_to(const T &obj, nos::ostream &os, const nos::buffer &opts)
        {
            (void)opts;
            return nos::print_implementation<T>::print_to(os, obj).value();
        }
    };

    template <typename T, bool HasMtdFPrint>
    struct fprint_implementation_solver<T, true, HasMtdFPrint>
    {
        static nos::expected<size_t, nos::output_error>
        fprint_to(const T &obj, nos::ostream &os, const nos::buffer &opts)
        {
            return *nos_fprint(adl_finder(os), obj, opts);
        }
    };

    template <typename T> struct fprint_implementation_solver<T, false, true>
    {
        static nos::expected<size_t, nos::output_error>
        fprint_to(const T &obj, nos::ostream &os, const nos::buffer &opts)
        {
            return *obj.fprint_to(os, opts);
        }
    };

    template <typename T>
    struct fprint_implementation
        : public fprint_implementation_solver<
              typename std::remove_cv<T>::type,
              nos::has_nos_fprint<typename std::remove_cv<T>::type>::value,
              nos::has_fprint_method<typename std::remove_cv<T>::type>::value>
    {
    };
}

#endif
