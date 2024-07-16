#ifndef IGRIS_TYPE_TRAITS_IMPL_IS_CONSTRUCTIBLE_H
#define IGRIS_TYPE_TRAITS_IMPL_IS_CONSTRUCTIBLE_H

namespace std
{

    /******* std::is_constructible replacement ******/
    template <typename T,
              typename AT_1 = void,
              typename AT_2 = void,
              typename AT_3 = void,
              typename AT_4 = void>
    class is_constructible_impl
    {
    private:
        template <typename C_T,
                  typename C_AT_1,
                  typename C_AT_2,
                  typename C_AT_3,
                  typename C_AT_4>
        static bool
        test(typename std::enable_if<
             sizeof(C_T) ==
             sizeof(C_T(
                 static_cast<C_AT_1>(
                     *static_cast<
                         typename std::remove_reference<C_AT_1>::type *>(NULL)),
                 static_cast<C_AT_2>(
                     *static_cast<
                         typename std::remove_reference<C_AT_2>::type *>(NULL)),
                 static_cast<C_AT_3>(
                     *static_cast<
                         typename std::remove_reference<C_AT_3>::type *>(NULL)),
                 static_cast<C_AT_4>(
                     *static_cast<typename std::remove_reference<C_AT_4>::type
                                      *>(NULL))))>::type *);

        template <typename, typename, typename, typename, typename>
        static int test(...);

    public:
        static const bool value =
            (sizeof(test<T, AT_1, AT_2, AT_3, AT_4>(NULL)) == sizeof(bool));
    };

    template <typename T, typename AT_1, typename AT_2, typename AT_3>
    class is_constructible_impl<T, AT_1, AT_2, AT_3, void>
    {
    private:
        template <typename C_T,
                  typename C_AT_1,
                  typename C_AT_2,
                  typename C_AT_3>
        static bool
        test(typename std::enable_if<
             sizeof(C_T) ==
             sizeof(C_T(
                 static_cast<C_AT_1>(
                     *static_cast<
                         typename std::remove_reference<C_AT_1>::type *>(NULL)),
                 static_cast<C_AT_2>(
                     *static_cast<
                         typename std::remove_reference<C_AT_2>::type *>(NULL)),
                 static_cast<C_AT_3>(
                     *static_cast<typename std::remove_reference<C_AT_3>::type
                                      *>(NULL))))>::type *);

        template <typename, typename, typename, typename> static int test(...);

    public:
        static const bool value =
            (sizeof(test<T, AT_1, AT_2, AT_3>(NULL)) == sizeof(bool));
    };

    template <typename T, typename AT_1, typename AT_2>
    class is_constructible_impl<T, AT_1, AT_2, void, void>
    {
    private:
        template <typename C_T, typename C_AT_1, typename C_AT_2>
        static bool
        test(typename std::enable_if<
             sizeof(C_T) ==
             sizeof(C_T(static_cast<C_AT_1>(
                            *static_cast<typename std::remove_reference<
                                C_AT_1>::type *>(NULL)),
                        static_cast<C_AT_2>(
                            *static_cast<typename std::remove_reference<
                                C_AT_2>::type *>(NULL))))>::type *);

        template <typename, typename, typename> static int test(...);

    public:
        static const bool value =
            (sizeof(test<T, AT_1, AT_2>(NULL)) == sizeof(bool));
    };

    template <typename T, typename AT_1>
    class is_constructible_impl<T, AT_1, void, void, void>
    {
    private:
        template <typename C_T, typename C_AT_1>
        static bool
        test(typename std::enable_if<
             sizeof(C_T) ==
             sizeof(C_T(static_cast<C_AT_1>(
                 *static_cast<typename std::remove_reference<C_AT_1>::type *>(
                     NULL))))>::type *);

        template <typename, typename> static int test(...);

    public:
        static const bool value = (sizeof(test<T, AT_1>(NULL)) == sizeof(bool));
    };

    template <typename T> class is_constructible_impl<T, void, void, void, void>
    {
    private:
        template <typename C_T> static C_T testFun(C_T);

        template <typename C_T>
        static bool
        test(typename std::enable_if<sizeof(C_T) ==
                                     sizeof(testFun(C_T()))>::type *);

        template <typename> static int test(...);

    public:
        static const bool value = (sizeof(test<T>(NULL)) == sizeof(bool));
    };

    template <typename T,
              typename AT_1 = void,
              typename AT_2 = void,
              typename AT_3 = void,
              typename AT_4 = void>
    class is_constructible_impl_ptr
    {
    public:
        static const bool value = false;
    };

    template <typename T, typename AT_1>
    class is_constructible_impl_ptr<
        T,
        AT_1,
        typename enable_if<
            is_pointer<typename remove_reference<T>::type>::value,
            void>::type,
        void,
        void>
    {
    private:
        template <typename C_T> static bool test(C_T);

        template <typename> static int test(...);

    public:
        static const bool value =
            (sizeof(test<T>(static_cast<AT_1>(NULL))) == sizeof(bool));
    };

    template <typename T>
    class is_constructible_impl_ptr<T, void, void, void, void>
    {
    public:
        static const bool value = true;
    };

    template <typename T,
              typename AT_1 = void,
              typename AT_2 = void,
              typename AT_3 = void,
              typename AT_4 = void>
    class is_constructible
    {
    public:
        static const bool value =
            (is_pointer<typename remove_reference<T>::type>::value
                 ? is_constructible_impl_ptr<T, AT_1, AT_2, AT_3, AT_4>::value
                 : is_constructible_impl<T, AT_1, AT_2, AT_3, AT_4>::value);
    };

}

#endif
