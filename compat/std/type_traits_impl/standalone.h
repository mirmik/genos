#ifndef IGRIS_STD_TYPE_TRAITS_STANDALONE_H
#define IGRIS_STD_TYPE_TRAITS_STANDALONE_H
#include "../igris_std_config.hpp"

#include <cstdint>
#include <cstdlib>
#include "../cstddef"

namespace IGRIS_STD_NS
{
    // detail::type_identify
    namespace detail
    {
        template <class T> struct type_identity
        {
            using type = T;
        }; // or use IGRIS_STD_NS::type_identity (since C++20)
    }

    // IGRIS_STD_NS::void_t
    template <class...> using void_t = void;

    // IGRIS_STD_NS::conditional
    template <bool B, class T, class F> struct conditional
    {
        typedef T type;
    };

    template <class T, class F> struct conditional<false, T, F>
    {
        typedef F type;
    };

    template <bool B, class T, class F>
    using conditional_t = typename conditional<B, T, F>::type;

    // IGRIS_STD_NS::enable_if
    template <bool B, class T = void> struct enable_if
    {
    };

    template <class T> struct enable_if<true, T>
    {
        typedef T type;
    };

    template <bool B, class T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    // IGRIS_STD_NS::integral_contant
    template <class T, T v> struct integral_constant
    {
        static constexpr T value = v;
        typedef T value_type;
        typedef integral_constant type;
        constexpr operator value_type() const noexcept
        {
            return value;
        }
        constexpr value_type operator()() const noexcept
        {
            return value;
        }
    };

    using true_type = IGRIS_STD_NS::integral_constant<bool, true>;
    using false_type = IGRIS_STD_NS::integral_constant<bool, false>;

    // IGRIS_STD_NS::is_same
    template <class T, class U> struct is_same : IGRIS_STD_NS::false_type
    {
    };

    template <class T> struct is_same<T, T> : IGRIS_STD_NS::true_type
    {
    };

    template <class T, class U> constexpr bool is_same_v = is_same<T, U>::value;

    // IGRIS_STD_NS::remove_***
    template <class T> struct remove_const
    {
        typedef T type;
    };
    template <class T> struct remove_const<const T>
    {
        typedef T type;
    };

    template <class T> struct remove_volatile
    {
        typedef T type;
    };
    template <class T> struct remove_volatile<volatile T>
    {
        typedef T type;
    };

    template <class T> struct remove_cv
    {
        typedef typename IGRIS_STD_NS::remove_volatile<
            typename IGRIS_STD_NS::remove_const<T>::type>::type type;
    };

    template <class T> using remove_cv_t = typename remove_cv<T>::type;

    template <class T> using remove_const_t = typename remove_const<T>::type;

    template <class T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template <class T> struct remove_reference
    {
        typedef T type;
    };
    template <class T> struct remove_reference<T &>
    {
        typedef T type;
    };
    template <class T> struct remove_reference<T &&>
    {
        typedef T type;
    };

    template <class T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <class T> struct remove_cvref
    {
        typedef IGRIS_STD_NS::remove_cv_t<IGRIS_STD_NS::remove_reference_t<T>> type;
    };

    template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

    template <class T> struct remove_extent
    {
        typedef T type;
    };

    template <class T> struct remove_extent<T[]>
    {
        typedef T type;
    };

    template <class T, IGRIS_STD_NS::size_t N> struct remove_extent<T[N]>
    {
        typedef T type;
    };

    template <class T> using remove_extent_t = typename remove_extent<T>::type;

    // IGRIS_STD_NS::add_pointer
    namespace detail
    {
        template <class T>
        auto try_add_pointer(int)
            -> type_identity<typename IGRIS_STD_NS::remove_reference<T>::type *>;
        template <class T> auto try_add_pointer(...) -> type_identity<T>;
    } // namespace detail

    template <class T>
    struct add_pointer : decltype(detail::try_add_pointer<T>(0))
    {
    };

    // IGRIS_STD_NS::add_rvalue_reference
    namespace detail
    {
        template <class T>
        auto try_add_lvalue_reference(int) -> type_identity<T &>;
        template <class T>
        auto try_add_lvalue_reference(...) -> type_identity<T>;

        template <class T>
        auto try_add_rvalue_reference(int) -> type_identity<T &&>;
        template <class T>
        auto try_add_rvalue_reference(...) -> type_identity<T>;
    } // namespace detail

    template <class T>
    struct add_lvalue_reference
        : decltype(detail::try_add_lvalue_reference<T>(0))
    {
    };

    template <class T>
    struct add_rvalue_reference
        : decltype(detail::try_add_rvalue_reference<T>(0))
    {
    };

    // Meta programming helper types.
    template <typename...> struct __or_;

    template <> struct __or_<> : public false_type
    {
    };

    template <typename _B1> struct __or_<_B1> : public _B1
    {
    };

    template <typename _B1, typename _B2>
    struct __or_<_B1, _B2> : public conditional<_B1::value, _B1, _B2>::type
    {
    };

    template <typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct __or_<_B1, _B2, _B3, _Bn...>
        : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type
    {
    };

    template <typename...> struct __and_;

    template <> struct __and_<> : public true_type
    {
    };

    template <typename _B1> struct __and_<_B1> : public _B1
    {
    };

    template <typename _B1, typename _B2>
    struct __and_<_B1, _B2> : public conditional<_B1::value, _B2, _B1>::type
    {
    };

    template <typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct __and_<_B1, _B2, _B3, _Bn...>
        : public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type
    {
    };

    template <typename _Pp>
    struct __not_ : public integral_constant<bool, !_Pp::value>
    {
    };

    template <IGRIS_STD_NS::size_t Len, IGRIS_STD_NS::size_t Align = sizeof(int)>
    struct aligned_storage
    {
        struct type
        {
            alignas(Align) unsigned char data[Len];
        };
    };
}

#endif
