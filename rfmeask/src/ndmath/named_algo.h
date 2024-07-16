#ifndef RABBIT_ND_NAMED_ALGO_H
#define RABBIT_ND_NAMED_ALGO_H

namespace ndmath
{
    template <class A, class B> class ab_converter
    {
        std::map<A, B> ab;
        std::map<B, A> ba;

    public:
        void add(A a, B b)
        {
            ab[a] = b;
            ba[b] = a;
        }

        B b(A idx) { return ab[idx]; }
        A a(B idx) { return ba[idx]; }

        B operator[](A idx) { return ab[idx]; }
        A operator[](B idx) { return ba[idx]; }
    };

    using int_string_converter = ab_converter<int, std::string>;
}

#endif