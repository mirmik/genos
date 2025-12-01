#ifndef IGRIS_UTIL_AB_CONVERTER_H
#define IGRIS_UTIL_AB_CONVERTER_H

#include <initializer_list>
#include <map>
#include <string>

namespace igris
{
    namespace detail
    {
        template <class A, class B, bool SameTypes> class ab_converter
        {
        protected:
            std::map<A, B> ab = {};
            std::map<B, A> ba = {};

        public:
            void add(const std::pair<A, B> &pair)
            {
                add(pair.first, pair.second);
            }

            void add(const A &a, const B &b)
            {
                ab[a] = b;
                ba[b] = a;
            }

            const B &b(const A &idx) const
            {
                return ab.at(idx);
            }
            const A &a(const B &idx) const
            {
                return ba.at(idx);
            }
        };

        template <class A, class B>
        class ab_converter<A, B, false> : public ab_converter<A, B, true>
        {
        public:
            const B &operator[](const A &idx) const
            {
                return this->ab.at(idx);
            }
            const A &operator[](const B &idx) const
            {
                return this->ba.at(idx);
            }
        };
    }

    template <class A, class B>
    class ab_converter
        : public detail::ab_converter<A, B, std::is_same<A, B>::value>
    {
    public:
        ab_converter() = default;

        ab_converter(const std::initializer_list<std::pair<A, B>> &pairs)
        {
            for (auto pair : pairs)
            {
                this->add(pair);
            }
        }
    };

    using int_string_converter = ab_converter<int, std::string>;
}

#endif