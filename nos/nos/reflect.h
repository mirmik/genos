#ifndef NOS_SERIALIZE_H
#define NOS_SERIALIZE_H

#include <string>
#include <string_view>

namespace nos
{
    class empty_reflect_operator
    {
    public:
        template <class T> bool reflect(const char *, T &)
        {
            return true;
        }
    };

    template <typename T> class has_reflect_method
    {
    private:
        template <typename U>
        static decltype(std::declval<U>().reflect(
                            std::declval<empty_reflect_operator &>()),
                        std::true_type())
        test(int);

        template <typename> static std::false_type test(...);

    public:
        static constexpr const bool value = decltype(test<T>(0))::value;
    };
}

#endif