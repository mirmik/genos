#ifndef IGRIS_STD_TYPE_TRAITS_DECAY_H
#define IGRIS_STD_TYPE_TRAITS_DECAY_H

#include <type_traits_impl/is.h>
#include <type_traits_impl/standalone.h>

namespace std
{
    namespace details
    {
        template <typename Base>
        std::true_type is_base_of_test_func(const volatile Base *);
        template <typename Base>
        std::false_type is_base_of_test_func(const volatile void *);
        template <typename Base, typename Derived>
        using pre_is_base_of =
            decltype(is_base_of_test_func<Base>(std::declval<Derived *>()));

        // with <experimental/type_traits>:
        // template <typename Base, typename Derived>
        // using pre_is_base_of2 =
        // std::experimental::detected_or_t<std::true_type,
        // pre_is_base_of, Base, Derived>;
        template <typename Base, typename Derived, typename = void>
        struct pre_is_base_of2 : public std::true_type
        {
        };
        // note std::void_t is a C++17 feature
        template <typename Base, typename Derived>
        struct pre_is_base_of2<Base,
                               Derived,
                               std::void_t<pre_is_base_of<Base, Derived>>>
            : public pre_is_base_of<Base, Derived>
        {
        };
    }

    template <typename Base, typename Derived>
    struct is_base_of
        : public std::conditional_t<std::is_class<Base>::value &&
                                        std::is_class<Derived>::value,
                                    details::pre_is_base_of2<Base, Derived>,
                                    std::false_type>
    {
    };

    template <class T> struct decay
    {
    private:
        typedef typename std::remove_reference<T>::type U;

    public:
        typedef typename std::conditional<
            std::is_array<U>::value,
            typename std::remove_extent<U>::type *,
            typename std::conditional<std::is_function<U>::value,
                                      typename std::add_pointer<U>::type,
                                      typename std::remove_cv<U>::type>::type>::
            type type;
    };
}

#endif
