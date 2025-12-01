#ifndef IGRIS_STD_TYPE_TRAITS_DECAY_H
#define IGRIS_STD_TYPE_TRAITS_DECAY_H
#include "../igris_std_config.hpp"

#include "is.h"
#include "standalone.h"

namespace IGRIS_STD_NS
{
    namespace details
    {
        template <typename Base>
        IGRIS_STD_NS::true_type is_base_of_test_func(const volatile Base *);
        template <typename Base>
        IGRIS_STD_NS::false_type is_base_of_test_func(const volatile void *);
        template <typename Base, typename Derived>
        using pre_is_base_of =
            decltype(is_base_of_test_func<Base>(IGRIS_STD_NS::declval<Derived *>()));

        // with <experimental/type_traits>:
        // template <typename Base, typename Derived>
        // using pre_is_base_of2 =
        // IGRIS_STD_NS::experimental::detected_or_t<IGRIS_STD_NS::true_type,
        // pre_is_base_of, Base, Derived>;
        template <typename Base, typename Derived, typename = void>
        struct pre_is_base_of2 : public IGRIS_STD_NS::true_type
        {
        };
        // note IGRIS_STD_NS::void_t is a C++17 feature
        template <typename Base, typename Derived>
        struct pre_is_base_of2<Base,
                               Derived,
                               IGRIS_STD_NS::void_t<pre_is_base_of<Base, Derived>>>
            : public pre_is_base_of<Base, Derived>
        {
        };
    }

    template <typename Base, typename Derived>
    struct is_base_of
        : public IGRIS_STD_NS::conditional_t<IGRIS_STD_NS::is_class<Base>::value &&
                                        IGRIS_STD_NS::is_class<Derived>::value,
                                    details::pre_is_base_of2<Base, Derived>,
                                    IGRIS_STD_NS::false_type>
    {
    };

    template <class T> struct decay
    {
    private:
        typedef typename IGRIS_STD_NS::remove_reference<T>::type U;

    public:
        typedef typename IGRIS_STD_NS::conditional<
            IGRIS_STD_NS::is_array<U>::value,
            typename IGRIS_STD_NS::remove_extent<U>::type *,
            typename IGRIS_STD_NS::conditional<IGRIS_STD_NS::is_function<U>::value,
                                      typename IGRIS_STD_NS::add_pointer<U>::type,
                                      typename IGRIS_STD_NS::remove_cv<U>::type>::type>::
            type type;
    };
}

#endif
