#ifndef GENOS_TUPLE
#define GENOS_TUPLE
#include "genos/gstl/utility.h"

template<int n, typename Head, typename ... Tail> 
class tuple_getter;

template<typename ... Tail> class tuple;

template<typename Head, typename... Tail>
struct tuple<Head, Tail...> : tuple<Tail...>
{
    tuple(Head h, Tail... tail)
	    : tuple<Tail...>(tail...), head(h) {}

    typedef tuple<Tail...> base_type;
    typedef Head           value_type;
        
    base_type& base = static_cast<base_type&>(*this);
    Head       head;
};

template<> class tuple<> {};



template<int I, typename Head, typename... Args>
struct getter
{
    typedef typename getter<I-1, Args...>::return_type return_type;
    static return_type get(tuple<Head, Args...> t)
    {
        return getter<I-1, Args...>::get(t);
    }
};

template<typename Head, typename... Args>
struct getter<0, Head, Args...>
{
    typedef typename tuple<Head, Args...>::value_type return_type;
    static return_type get(tuple<Head, Args...> t)
    {
        return t.head;
    }
};

template<int I, typename Head, typename... Args>
typename getter<I, Head, Args...>::return_type
get(tuple<Head, Args...> t)
{
    return getter<I, Head, Args...>::get(t);
}



/*
template<int n, typename Head, typename ... Tail> 
typename tuple_getter<n, Head, Tail ...>::return_type 
tuple_get(tuple<Head,Tail...>&& tpl) 
{ 
	return tuple_getter<n, Head, Tail ...>::get(tpl); 
};*/

/*
template<typename F, typename Tuple, bool Enough, int TotalArgs, int... N>
struct call_impl
{
    auto static call(F f, Tuple&& t)
    {
        return call_impl<F, Tuple, TotalArgs == 1 + sizeof...(N), 
                                   TotalArgs, N..., sizeof...(N)
                        >::call(f, gxx::forward<Tuple>(t));
    }
};

template<typename F, typename Tuple, int TotalArgs, int... N>
struct call_impl<F, Tuple, true, TotalArgs, N...>
{
    auto static call(F f, Tuple&& t)
    {
        return f(tuple_get<N>(gxx::forward<Tuple>(t))...);
    }
};
*/
/*
template<typename F, typename Tuple>
auto call(F f, Tuple&& t)
{
    typedef typename gxx::decay<Tuple>::type type;
    return call_impl<F, Tuple, 0 == gxx::tuple_size<type>::value, 
                                    gxx::tuple_size<type>::value
                    >::call(f, gxx::forward<Tuple>(t));
}
*/
#endif