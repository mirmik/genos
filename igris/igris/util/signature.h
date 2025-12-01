// Утилиты для анализа классов

// Check class for attribute existing.
// Example:
//     class A { void a() {} };
//     class B { void b() {} };
//
//     IGRIS_SIGNATURE_ATTRIBUTE_CHECKER(has_a, a)
//
//     ...
//     has_a<A>() -> true
//     has_a<B>() -> false
#define IGRIS_SIGNATURE_ATTRIBUTE_CHECKER(name, attr)                          \
    template <class T> class name##_checker_class                              \
    {                                                                          \
    private:                                                                   \
        template <class U>                                                     \
        static decltype(&U::attr, std::true_type()) test(int);                 \
        template <class> static std::false_type test(...);                     \
                                                                               \
    public:                                                                    \
        static constexpr const bool value = decltype(test<T>(0))::value;       \
    };                                                                         \
                                                                               \
    template <class T> constexpr bool name()                                   \
    {                                                                          \
        return name##_checker_class<typename std::remove_cv<T>::type>::value;  \
    }
