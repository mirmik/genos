


template<class T> struct make_unsigned					{typedef T type;};
template<> struct make_unsigned<int>				{typedef unsigned int type;};
template<> struct make_unsigned<char>			{typedef unsigned char type;};
template<> struct make_unsigned<long>			{typedef unsigned long type;};
template<> struct make_unsigned<short>			{typedef unsigned short type;};
template<> struct make_unsigned<long long>		{typedef unsigned long long type;};


//remove_volatile
template<class T> struct remove_volatile					{	typedef T type;	  };
template<class T> struct remove_volatile<volatile T>		{  	typedef T type;   };

//remove_const
template<class T> struct remove_const					{	typedef T type;	  };
template<class T> struct remove_const<const T>			{  	typedef T type;   };

//remove_cv
template< class T >
struct remove_cv {
	typedef typename gxx::remove_volatile<typename gxx::remove_const<T>::type>::type type;
};




//remove_reference
template< class T > struct remove_reference      {typedef T type;};
template< class T > struct remove_reference<T&>  {typedef T type;};
template< class T > struct remove_reference<T&&> {typedef T type;};



//add_lvalue_reference //add_rvalue_reference
template< class T > struct add_lvalue_reference			{  typedef T& type;   };
template< class T > struct add_rvalue_reference			{  typedef T&& type;  };
template< class T > struct add_reference				{  typedef T& type;   };

//add_const
template< class T> struct add_const { typedef const T type; };

//add_volatile
template< class T> struct add_volatile { typedef volatile T type; };

//add_cv
template< class T >
struct add_cv {
	typedef typename gxx::add_volatile<typename gxx::add_const<T>::type>::type type;
};


//add_pointer
namespace detail {
	template< class T, bool is_function_type = false >
	struct add_pointer {
		using type = typename gxx::remove_reference<T>::type*;
	};
	
	template< class T >
	struct add_pointer<T, true> {
		using type = T;
	};
	
	template< class T, class... Args >
	struct add_pointer<T(Args...), true> {	//Интересно, зачем это тут...
		using type = T(*)(Args...);
	};
	
	template< class T, class... Args >
	struct add_pointer<T(Args..., ...), true> { 	//Интересно, зачем это тут...
		using type = T(*)(Args..., ...);
	};
	
} // namespace detail

template< class T >
struct add_pointer : detail::add_pointer<T, gxx::is_function<T>::value> {};






//remove_extent Работа с масивами. Вычисляет базовый тип масива.
template<class T>
struct remove_extent { typedef T type; };
 
template<class T>
struct remove_extent<T[]> { typedef T type; };
 
template<class T, size_t N>
struct remove_extent<T[N]> { typedef T type;};


//remove_all_extents Аналогично для мультиразмерного массива.
template<class T>
struct remove_all_extents { typedef T type;};
 
template<class T>
struct remove_all_extents<T[]> {
    typedef typename remove_all_extents<T>::type type;
};
 
template<class T, size_t N>
struct remove_all_extents<T[N]> {
    typedef typename remove_all_extents<T>::type type;
};


/*
//aligned_storage		Интересно, что это оно делает???
template<size_t Len, size_t Align /.. default alignment not implemented >
struct aligned_storage {
    typedef struct {
        alignas(Align) unsigned char data[Len];
    } type;
};

//aligned_union
template <size_t Len, class... Types>
struct aligned_union
{
    static constexpr size_t alignment_value = gxx::max({alignof(Types)...});
 
    struct type
    {
      alignas(alignment_value) char _s[gxx::max({Len, sizeof(Types)...})];
    };
};
*/


