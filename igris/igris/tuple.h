#ifndef IGRIS_TUPLE_H
#define IGRIS_TUPLE_H

namespace igris
{
    template <class... Args> struct tuple
    {
    };

    template <class A> struct tuple<A>
    {
        A a;
        tuple(A a) : a(a) {}
    };
    template <class A, class B> struct tuple<A, B>
    {
        A a;
        B b;
        tuple(A a, B b) : a(a), b(b) {}
    };
    template <class A, class B, class C> struct tuple<A, B, C>
    {
        A a;
        B b;
        C c;
        tuple(A a, B b, C c) : a(a), b(b), c(c) {}
    };
    template <class A, class B, class C, class D> struct tuple<A, B, C, D>
    {
        A a;
        B b;
        C c;
        D d;
        tuple(A a, B b, C c, D d) : a(a), b(b), c(c), d(d) {}
    };
    template <class A, class B, class C, class D, class E>
    struct tuple<A, B, C, D, E>
    {
        A a;
        B b;
        C c;
        D d;
        E e;
        tuple(A a, B b, C c, D d, E e) : a(a), b(b), c(c), d(d), e(e) {}
    };

    template <int N, class... Args> struct get_impl
    {
    };
    template <class... Args> struct get_impl<0, Args...>
    {
        constexpr static auto &doit(igris::tuple<Args...> &arg)
        {
            return arg.a;
        }
    };
    template <class... Args> struct get_impl<1, Args...>
    {
        constexpr static auto &doit(igris::tuple<Args...> &arg)
        {
            return arg.b;
        }
    };
    template <class... Args> struct get_impl<2, Args...>
    {
        constexpr static auto &doit(igris::tuple<Args...> &arg)
        {
            return arg.c;
        }
    };
    template <class... Args> struct get_impl<3, Args...>
    {
        constexpr static auto &doit(igris::tuple<Args...> &arg)
        {
            return arg.d;
        }
    };
    template <class... Args> struct get_impl<4, Args...>
    {
        constexpr static auto &doit(igris::tuple<Args...> &arg)
        {
            return arg.e;
        }
    };

    template <int N, class... Args> auto get(igris::tuple<Args...> &args)
    {
        return igris::get_impl<N, Args...>::doit(args);
    }
}

namespace std
{
    template <int N, class... Args> auto get(igris::tuple<Args...> &args)
    {
        return igris::get<N>(args);
    }
}

#endif
