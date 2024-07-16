#ifndef IGRIS_STD_TYPE_TRAITS_IS_H
#define IGRIS_STD_TYPE_TRAITS_IS_H

#include <cstddef>
#include <type_traits_impl/standalone.h>

namespace std
{
    template <typename> struct __is_void_helper : public false_type
    {
    };

    template <> struct __is_void_helper<void> : public true_type
    {
    };

    /// is_void
    template <typename _Tp>
    struct is_void
        : public __is_void_helper<typename remove_cv<_Tp>::type>::type
    {
    };

    template <typename> struct __is_integral_helper : public false_type
    {
    };

    template <> struct __is_integral_helper<bool> : public true_type
    {
    };

    template <> struct __is_integral_helper<char> : public true_type
    {
    };

    template <> struct __is_integral_helper<signed char> : public true_type
    {
    };

    template <> struct __is_integral_helper<unsigned char> : public true_type
    {
    };

#ifdef _GLIBCXX_USE_WCHAR_T
    template <> struct __is_integral_helper<wchar_t> : public true_type
    {
    };
#endif

    template <> struct __is_integral_helper<char16_t> : public true_type
    {
    };

    template <> struct __is_integral_helper<char32_t> : public true_type
    {
    };

    template <> struct __is_integral_helper<short> : public true_type
    {
    };

    template <> struct __is_integral_helper<unsigned short> : public true_type
    {
    };

    template <> struct __is_integral_helper<int> : public true_type
    {
    };

    template <> struct __is_integral_helper<unsigned int> : public true_type
    {
    };

    template <> struct __is_integral_helper<long> : public true_type
    {
    };

    template <> struct __is_integral_helper<unsigned long> : public true_type
    {
    };

    template <> struct __is_integral_helper<long long> : public true_type
    {
    };

    template <>
    struct __is_integral_helper<unsigned long long> : public true_type
    {
    };

    // Conditionalizing on __STRICT_ANSI__ here will break any port that
    // uses one of these types for size_t.
#if defined(__GLIBCXX_TYPE_INT_N_0)
    template <>
    struct __is_integral_helper<__GLIBCXX_TYPE_INT_N_0> : public true_type
    {
    };

    template <>
    struct __is_integral_helper<unsigned __GLIBCXX_TYPE_INT_N_0>
        : public true_type
    {
    };
#endif
#if defined(__GLIBCXX_TYPE_INT_N_1)
    template <>
    struct __is_integral_helper<__GLIBCXX_TYPE_INT_N_1> : public true_type
    {
    };

    template <>
    struct __is_integral_helper<unsigned __GLIBCXX_TYPE_INT_N_1>
        : public true_type
    {
    };
#endif
#if defined(__GLIBCXX_TYPE_INT_N_2)
    template <>
    struct __is_integral_helper<__GLIBCXX_TYPE_INT_N_2> : public true_type
    {
    };

    template <>
    struct __is_integral_helper<unsigned __GLIBCXX_TYPE_INT_N_2>
        : public true_type
    {
    };
#endif
#if defined(__GLIBCXX_TYPE_INT_N_3)
    template <>
    struct __is_integral_helper<__GLIBCXX_TYPE_INT_N_3> : public true_type
    {
    };

    template <>
    struct __is_integral_helper<unsigned __GLIBCXX_TYPE_INT_N_3>
        : public true_type
    {
    };
#endif

    /// is_integral
    template <typename _Tp>
    struct is_integral
        : public __is_integral_helper<typename remove_cv<_Tp>::type>::type
    {
    };

    template <typename> struct __is_floating_point_helper : public false_type
    {
    };

    template <> struct __is_floating_point_helper<float> : public true_type
    {
    };

    template <> struct __is_floating_point_helper<double> : public true_type
    {
    };

    template <>
    struct __is_floating_point_helper<long double> : public true_type
    {
    };

#if !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_FLOAT128)
    template <> struct __is_floating_point_helper<__float128> : public true_type
    {
    };
#endif

    /// is_floating_point
    template <typename _Tp>
    struct is_floating_point
        : public __is_floating_point_helper<typename remove_cv<_Tp>::type>::type
    {
    };

    /// is_array
    template <typename> struct is_array : public false_type
    {
    };

    template <typename _Tp, std::size_t _Size>
    struct is_array<_Tp[_Size]> : public true_type
    {
    };

    template <typename _Tp> struct is_array<_Tp[]> : public true_type
    {
    };

    template <typename> struct __is_pointer_helper : public false_type
    {
    };

    template <typename _Tp> struct __is_pointer_helper<_Tp *> : public true_type
    {
    };

    /// is_pointer
    template <typename _Tp>
    struct is_pointer
        : public __is_pointer_helper<typename remove_cv<_Tp>::type>::type
    {
    };

    /// is_lvalue_reference
    template <typename> struct is_lvalue_reference : public false_type
    {
    };

    template <typename _Tp> struct is_lvalue_reference<_Tp &> : public true_type
    {
    };

    /// is_rvalue_reference
    template <typename> struct is_rvalue_reference : public false_type
    {
    };

    template <typename _Tp>
    struct is_rvalue_reference<_Tp &&> : public true_type
    {
    };

    template <typename> struct is_function;

    template <typename>
    struct __is_member_object_pointer_helper : public false_type
    {
    };

    template <typename _Tp, typename _Cp>
    struct __is_member_object_pointer_helper<_Tp _Cp::*>
        : public integral_constant<bool, !is_function<_Tp>::value>
    {
    };

    /// is_member_object_pointer
    template <typename _Tp>
    struct is_member_object_pointer : public __is_member_object_pointer_helper<
                                          typename remove_cv<_Tp>::type>::type
    {
    };

    template <typename>
    struct __is_member_function_pointer_helper : public false_type
    {
    };

    template <typename _Tp, typename _Cp>
    struct __is_member_function_pointer_helper<_Tp _Cp::*>
        : public integral_constant<bool, is_function<_Tp>::value>
    {
    };

    /// is_member_function_pointer
    template <typename _Tp>
    struct is_member_function_pointer
        : public __is_member_function_pointer_helper<
              typename remove_cv<_Tp>::type>::type
    {
    };

    /// is_enum
    template <typename _Tp>
    struct is_enum : public integral_constant<bool, __is_enum(_Tp)>
    {
    };

    /// is_union
    template <typename _Tp>
    struct is_union : public integral_constant<bool, __is_union(_Tp)>
    {
    };

    /// is_class
    template <typename _Tp>
    struct is_class : public integral_constant<bool, __is_class(_Tp)>
    {
    };

    /// is_function
    template <typename> struct is_function : public false_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...)> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) &> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) &&> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......)> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) &> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) &&> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const &> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const &&> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const &> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const &&> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) volatile> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) volatile &> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) volatile &&> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) volatile> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) volatile &> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) volatile &&> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const volatile> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const volatile &> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const volatile &&> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const volatile> : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const volatile &>
        : public true_type
    {
    };

    template <typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const volatile &&>
        : public true_type
    {
    };

#define __cpp_lib_is_null_pointer 201309

    template <typename> struct __is_null_pointer_helper : public false_type
    {
    };

    template <>
    struct __is_null_pointer_helper<std::nullptr_t> : public true_type
    {
    };

    /// is_null_pointer (LWG 2247).
    template <typename _Tp>
    struct is_null_pointer
        : public __is_null_pointer_helper<typename remove_cv<_Tp>::type>::type
    {
    };

    /// __is_nullptr_t (extension).
    template <typename _Tp> struct __is_nullptr_t : public is_null_pointer<_Tp>
    {
    };

    // Composite type categories.

    /// is_reference
    template <typename _Tp>
    struct is_reference
        : public __or_<is_lvalue_reference<_Tp>, is_rvalue_reference<_Tp>>::type
    {
    };

    /// is_arithmetic
    template <typename _Tp>
    struct is_arithmetic
        : public __or_<is_integral<_Tp>, is_floating_point<_Tp>>::type
    {
    };

    /// is_fundamental
    template <typename _Tp>
    struct is_fundamental : public __or_<is_arithmetic<_Tp>,
                                         is_void<_Tp>,
                                         is_null_pointer<_Tp>>::type
    {
    };

    /// is_object
    template <typename _Tp>
    struct is_object
        : public __not_<
              __or_<is_function<_Tp>, is_reference<_Tp>, is_void<_Tp>>>::type
    {
    };

    template <typename> struct is_member_pointer;

    /// is_scalar
    template <typename _Tp>
    struct is_scalar : public __or_<is_arithmetic<_Tp>,
                                    is_enum<_Tp>,
                                    is_pointer<_Tp>,
                                    is_member_pointer<_Tp>,
                                    is_null_pointer<_Tp>>::type
    {
    };

    /// is_compound
    template <typename _Tp>
    struct is_compound
        : public integral_constant<bool, !is_fundamental<_Tp>::value>
    {
    };

    template <typename _Tp>
    struct __is_member_pointer_helper : public false_type
    {
    };

    template <typename _Tp, typename _Cp>
    struct __is_member_pointer_helper<_Tp _Cp::*> : public true_type
    {
    };

    /// is_member_pointer
    template <typename _Tp>
    struct is_member_pointer
        : public __is_member_pointer_helper<typename remove_cv<_Tp>::type>::type
    {
    };

    // Utility to detect referenceable types ([defns.referenceable]).

    template <typename _Tp>
    struct __is_referenceable
        : public __or_<is_object<_Tp>, is_reference<_Tp>>::type
    {
    };

    template <typename _Res, typename... _Args>
    struct __is_referenceable<_Res(_Args...)> : public true_type
    {
    };

    template <typename _Res, typename... _Args>
    struct __is_referenceable<_Res(_Args......)> : public true_type
    {
    };

    // Type properties.

    /// is_const
    template <typename> struct is_const : public false_type
    {
    };

    template <typename _Tp> struct is_const<_Tp const> : public true_type
    {
    };

    /// is_volatile
    template <typename> struct is_volatile : public false_type
    {
    };

    template <typename _Tp> struct is_volatile<_Tp volatile> : public true_type
    {
    };

    /*
    /// is_trivial
    template<typename _Tp>
        struct is_trivial
        : public integral_constant<bool, __is_trivial(_Tp)>
        { };

    // is_trivially_copyable
    template<typename _Tp>
        struct is_trivially_copyable
        : public integral_constant<bool, __is_trivially_copyable(_Tp)>
        { };*/

    /// is_standard_layout
    template <typename _Tp>
    struct is_standard_layout
        : public integral_constant<bool, __is_standard_layout(_Tp)>
    {
    };

    /// is_pod
    // Could use is_standard_layout && is_trivial instead of the builtin.
    template <typename _Tp>
    struct is_pod : public integral_constant<bool, __is_pod(_Tp)>
    {
    };

    /// is_literal_type
    template <typename _Tp>
    struct is_literal_type
        : public integral_constant<bool, __is_literal_type(_Tp)>
    {
    };

    /// is_empty
    template <typename _Tp>
    struct is_empty : public integral_constant<bool, __is_empty(_Tp)>
    {
    };

    /// is_polymorphic
    template <typename _Tp>
    struct is_polymorphic
        : public integral_constant<bool, __is_polymorphic(_Tp)>
    {
    };

#if __cplusplus >= 201402L
#define __cpp_lib_is_final 201402L
    /// is_final
    template <typename _Tp>
    struct is_final : public integral_constant<bool, __is_final(_Tp)>
    {
    };
#endif

    /// is_abstract
    template <typename _Tp>
    struct is_abstract : public integral_constant<bool, __is_abstract(_Tp)>
    {
    };

    template <typename _Tp, bool = is_arithmetic<_Tp>::value>
    struct __is_signed_helper : public false_type
    {
    };

    template <typename _Tp>
    struct __is_signed_helper<_Tp, true>
        : public integral_constant<bool, _Tp(-1) < _Tp(0)>
    {
    };

    /// is_signed
    template <typename _Tp>
    struct is_signed : public __is_signed_helper<_Tp>::type
    {
    };

    /// is_unsigned
    template <typename _Tp>
    struct is_unsigned
        : public __and_<is_arithmetic<_Tp>, __not_<is_signed<_Tp>>>::type
    {
    };
}

#endif
