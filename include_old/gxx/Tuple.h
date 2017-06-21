#ifndef GXX_TUPLE_H
#define GXX_TUPLE_H

namespace gxx {

	template <int Deep, typename T> class TupleBase {
	public:
		T data;
		TupleBase() : data() {};
		TupleBase(const T& value) : data(value) {};
		T baseValue() const {return data;};
		T& baseReference() {return data;};
	}; 



	template <int Deep, typename Head, typename ... Tail>
	class TupleImpl : public TupleImpl<Deep-1,Tail ...>, public TupleBase<Deep,Head> {
	public:
		using Parent = TupleImpl<Deep-1,Tail...>;
		using Base = TupleBase<Deep,Head>;
	
		//Tuple2() : Parent(), top() {}
		TupleImpl(Head head, Tail ... tail) : Parent(tail ...), Base(head) {}
		TupleImpl() : Parent(), Base() {}

		static constexpr int deep = Deep;
		Head value() const {return Base::baseValue();};	
		Head& reference() {return Base::baseReference();};

		constexpr TupleImpl& get_impl() {return (TupleImpl&)*this;};
	};
	
	template <typename Head>
	class TupleImpl <1,Head> : public TupleBase<1,Head> {
		using Base = TupleBase<1,Head>;
	public:	
		//Tuple2() : top() {};
		TupleImpl(Head head) : Base(head) {};
		TupleImpl() : Base() {};
		static constexpr int deep = 1;
		Head value() const {return Base::baseValue();};
		Head& reference() {return Base::baseReference();};

		constexpr TupleImpl& get_impl() {return (TupleImpl&)*this;};
	};



	template <typename Head, typename ... Tail>
	class Tuple : public TupleImpl<sizeof...(Tail) + 1, Head, Tail ...> {
	public:
		using Parent = TupleImpl<sizeof...(Tail) + 1, Head, Tail ...>;
		Tuple(Head head, Tail ... tail) : Parent(head, tail ...) {};
		Tuple() : Parent() {};		
	};
	
	template <typename Head>
	class Tuple <Head> : public TupleImpl<1,Head> {
	public:
		using Parent = TupleImpl<1,Head>;
		Tuple() : Parent() {};	
		Tuple(Head head) : Parent(head) {};	
	};

	template <typename Tpl>
	class tuple_size {
	public:
		static constexpr int value = Tpl::Parent::deep;
	};


	template <int Index, typename Head, typename ... Tail>
	class GetImpl {
	public:
		static auto get_value(TupleImpl<sizeof...(Tail) + 1,Head,Tail...> &t) -> decltype(GetImpl<Index - 1, Tail...>::get_value(t)) {
			return GetImpl<Index - 1, Tail...>::get_value(t);
		}
	};
	
	template <typename Head, typename ... Tail>
	class GetImpl<0,Head,Tail...> {
	public:
		static Head& get_value(TupleImpl<sizeof...(Tail) + 1,Head,Tail...> &t) {
			return t.reference();
		}
	};
	
	template <int Index, typename Head, typename ... Tail>
	auto get(Tuple<Head,Tail...>& t) -> decltype(GetImpl<Index,Head,Tail...>::get_value(t)) {
		return GetImpl<Index,Head,Tail...>::get_value(t.get_impl());
	}	
	
	template <typename ... Args>
	Tuple<Args&...> tie (Args& ... args) {
		return Tuple<Args&...>(args...);
	}


	// Главная роль здесь у шаблона класса iterate_tuple.
    
    // Первый параметр шаблона класса iterate_tuple имеет тип int (index).
    // Значение этого параметра используется функцией get, 
    // которая "достает" из кортежа элемент по указанной позиции.
    
    // Мы рекурсивно сдвигаем позицию (уменьшаем index на 1) и таким образом
    // перемещаемся по кортежу.
    
    // Когда значение индекса становится равно 0, рекурсия завершается,
    // благодаря частичной специализации для индекса равного 0.
    
    // При этом есть особый случай, когда индекс равен -1. Он соответствует 
    // ситуации, когда кортеж не содержит ни одного элемента.
    
	template<int index, typename Callback, typename... Args>
	struct iterate_tuple 
	{
		static void next(Tuple<Args...>& t, Callback callback) 
		{
			// Уменьшаем позицию и рекурсивно вызываем этот же метод 
			iterate_tuple<index - 1, Callback, Args...>::next(t, callback);
			
			// Вызываем обработчик и передаем ему позицию и значение элемента
			callback(index, gxx::get<index>(t));
		}
	};
	
	// Частичная специализация для индекса 0 (завершает рекурсию)
	template<typename Callback, typename... Args>
	struct iterate_tuple<0, Callback, Args...> 
	{
		static void next(Tuple<Args...>& t, Callback callback) 
		{
			callback(0, gxx::get<0>(t));
		}
	};

	// Частичная специализация для индекса -1 (пустой кортеж)
	template<typename Callback, typename... Args>
	struct iterate_tuple<-1, Callback, Args...>
	{
		static void next(Tuple<Args...>& t, Callback callback)
		{
			// ничего не делаем
		}
	};


	template<typename Callback, typename... Args>
	void tuple_for_each(gxx::Tuple<Args...>& t, Callback callback) 
	{
	    // Размер кортежа
		int const t_size = tuple_size<gxx::Tuple<Args...>>::value;
    
 	   	// Запускаем рекурсивный обход элементов кортежа во время компиляции
		iterate_tuple<t_size - 1, Callback, Args...>::next(t, callback);
	}




};

#endif