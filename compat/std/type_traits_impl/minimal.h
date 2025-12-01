#ifndef IGRIS_STD_TYPE_TRAITS_MINIMAL_H
#define IGRIS_STD_TYPE_TRAITS_MINIMAL_H
#include "../igris_std_config.hpp"

#include "standalone.h"
#include "../utility_impl/minimal.h"

namespace IGRIS_STD_NS
{
    template <class T> struct add_const
    {
        typedef const T type;
    };
    template <class T> struct add_volatile
    {
        typedef volatile T type;
    };

    template <class T> struct add_cv
    {
        typedef
            typename IGRIS_STD_NS::add_volatile<typename IGRIS_STD_NS::add_const<T>::type>::type
                type;
    };

    typedef char small_;

    struct big_
    {
        char dummy[2];
    };

    namespace detail
    {

        // This class is an implementation detail for is_convertible, and you
        // don't need to know how it works to use is_convertible. For those
        // who care: we declare two different functions, one whose argument is
        // of type To and one with a variadic argument list. We give them
        // return types of different size, so we can use sizeof to trick the
        // compiler into telling us which function it would have chosen if we
        // had called it with an argument of type From.  See Alexandrescu's
        // _Modern C++ Design_ for more details on this sort of trick.

        template <typename From, typename To> struct ConvertHelper
        {
            static small_ Test(To);
            static big_ Test(...);
            static From Create();
        };
    } // namespace detail

    // Inherits from true_type if From is convertible to To, false_type
    // otherwise.
    template <typename From, typename To>
    struct is_convertible
        : integral_constant<bool,
                            sizeof(detail::ConvertHelper<From, To>::Test(
                                detail::ConvertHelper<From, To>::Create())) ==
                                sizeof(small_)>
    {
    };

    // fallback helper for when the true case won't compile - returns false_type
    template <class T, class From> IGRIS_STD_NS::false_type is_assignable_impl(...)
    {
        return {};
    }

    // this overload preferred if decltype(T = From) yields a type
    // returns a true_type
    template <class T, class From>
    auto is_assignable_impl(int)
        -> decltype(IGRIS_STD_NS::declval<T>() = IGRIS_STD_NS::declval<From>(),
                    void(),
                    IGRIS_STD_NS::true_type())
    {
        return {};
    }

    // derived from either false_type or true_type
    template <class T, class From>
    struct is_assignable : decltype(is_assignable_impl<T, From>(0))
    {
    };

}

#endif
