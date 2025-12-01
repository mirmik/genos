#ifndef IGRIS_STD_FUNCTIONAL_REFERENCE_WRAPPER_H
#define IGRIS_STD_FUNCTIONAL_REFERENCE_WRAPPER_H
#include "../igris_std_config.hpp"

namespace IGRIS_STD_NS
{
    template <class T> T *reference_wrapper_addressof(T &ref)
    {
        return &ref;
    }

    template <class T> T *reference_wrapper_addressof(const T &ref)
    {
        return &const_cast<T &>(ref);
    }

    namespace detail
    {
        template <class T> T &FUN(T &t) noexcept
        {
            return t;
        }
        template <class T> void FUN(T &&) = delete;
    }

    template <class T> class reference_wrapper
    {
    public:
        // types
        typedef T type;

        // construct/copy/destroy
        template <class U,
                  class = decltype(
                      detail::FUN<T>(IGRIS_STD_NS::declval<U>()),
                      IGRIS_STD_NS::enable_if_t<!IGRIS_STD_NS::is_same_v<reference_wrapper,
                                                       remove_cvref_t<U>>>())>
        reference_wrapper(U &&u) noexcept(
            noexcept(detail::FUN<T>(IGRIS_STD_NS::forward<U>(u))))
            : _ptr(reference_wrapper_addressof(
                  detail::FUN<T>(IGRIS_STD_NS::forward<U>(u))))
        {
        }
        reference_wrapper(const reference_wrapper &) noexcept = default;

        // assignment
        reference_wrapper &
        operator=(const reference_wrapper &x) noexcept = default;

        // access
        operator T &() const noexcept
        {
            return *_ptr;
        }
        T &get() const noexcept
        {
            return *_ptr;
        }

        /*template <class... ArgTypes>
        IGRIS_STD_NS::invoke_result_t<T &, ArgTypes...>
        operator()(ArgTypes &&... args) const
        {
            return IGRIS_STD_NS::invoke(get(), IGRIS_STD_NS::forward<ArgTypes>(args)...);
        }*/

    private:
        T *_ptr;
    };

    // deduction guides
    // template <class T> reference_wrapper(T &) -> reference_wrapper<T>;
}

#endif
