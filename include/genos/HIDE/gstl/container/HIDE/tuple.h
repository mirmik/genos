#ifndef GENOS_TUPLE
	#define GENOS_TUPLE
	
	#include "genos/gstl/utility.h"
	
	using namespace gxx;
	
		template<typename... Args>
		struct tuple;
		
		
		
		
		
		
		template<typename Head, typename... Tail>
		struct tuple<Head, Tail...> : tuple<Tail...>
		{
			tuple(Head h, Tail... tail)
            : tuple<Tail...>(tail...), head_(h)
			{}
			
			static constexpr int size(){return (sizeof ... (Tail) + 1); };
			tuple(){};
			
			typedef tuple<Tail...> base_type;
			typedef Head           value_type;
			
			base_type& base = static_cast<base_type&>(*this);
			Head       head_;
		};
		
		
		template<>
		struct tuple<>
		{
			static constexpr int size(){return 0;};
		};
		
		
		
		
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
				return t.head_;
			}
		};
		
		
		
		template<int I, typename Head, typename... Args>
		typename getter<I, Head, Args...>::return_type
		get(tuple<Head, Args...> t)
		{
			return getter<I, Head, Args...>::get(t);
		}
		
		
		
		
		
		
		
		
template< class T >
struct decay;	
		
		
template< class T, T v >
struct integral_constant;
		
		
		
		
template< class T >
class tuple_size; /*undefined*/

template< class... Types >
class tuple_size< tuple<Types...> >
 : public integral_constant<size_t, sizeof...(Types)> { };
 
template< class T >
class tuple_size<const T>
 : public integral_constant<size_t, tuple_size<T>::value> { };
 
template< class T >
class tuple_size< volatile T >
 : public integral_constant<size_t, tuple_size<T>::value> { };
 
template< class T >
class tuple_size< const volatile T >
 : public integral_constant<size_t, tuple_size<T>::value> { };
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		template<typename F, typename Tuple, bool Enough, int TotalArgs, int... N>
		struct call_impl
		{
			auto static call(F f, Tuple&& t)
			{
				return call_impl<F, Tuple, TotalArgs == 1 + sizeof...(N), 
				TotalArgs, N..., sizeof...(N)
				>::call(f, forward<Tuple>(t));
			}
		};
		
		
		
		
		
		
		template<typename F, typename Tuple, int TotalArgs, int... N>
		struct call_impl<F, Tuple, true, TotalArgs, N...>
		{
			auto static call(F f, Tuple&& t)
			{
				return f(get<N>(forward<Tuple>(t))...);
			}
		};
		
		
		
		
		
		template<typename F, typename Tuple>
		auto call(F f, Tuple&& t)
		{
			typedef typename gxx::decay<Tuple>::type type;
			return call_impl<F, Tuple, 0 == tuple_size<type>::value, 
			tuple_size<type>::value
			>::call(f, forward<Tuple>(t));
		}
		
		
		
		
		
#endif