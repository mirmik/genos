


//Реализации шаблонов разнесены по файлам, сгруппированы по выполняемым функциям
//Шаблоны, меж тем, порой сложно организованны, перекрёстно ссылаются друг на друга
//Дабы они не ругались, немогучи найти сдельщиков, здесь объявлены прототипы.
//Может оказаться, что здесь они не все.

//TODO Оглавление по файлам.

template <class> struct is_function;
template <class T> struct is_integral;
template <class T> struct is_floating_point;
template <class T> struct is_pointer;
template <class T> struct is_enum;
template <class T> struct is_reference;
template <class T> struct is_pod;
template <class T> struct has_trivial_constructor;
template <class T> struct has_trivial_copy;
template <class T> struct has_trivial_assign;
template <class T> struct has_trivial_destructor;
template <class T, class U> struct is_same;
template <class From, class To> struct is_convertible;
template< class T > struct is_member_function_pointer;
template< class T > struct is_scalar;
template<class T, T v> struct integral_constant;

template<bool B, class T, class F>	struct conditional;

template<class T> struct remove_volatile;	
template<class T> struct remove_const;		
template< class T > struct remove_cv; 
template< class T > struct add_reference;			

template< class T > struct remove_reference;
template< class T > struct add_lvalue_reference;
template< class T > struct add_rvalue_reference;
template< class T>  struct add_const;
template< class T>  struct add_volatile;
template< class T > struct add_cv;
template< class T > struct add_pointer;
