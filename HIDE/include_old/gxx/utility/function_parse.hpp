template<typename Function> 
struct signature0;

template<typename Ret> 
struct signature0<Ret(*)()> {
	using return_type = Ret;
	using function_type = Ret(*)();
};

template<typename Function> 
struct signature1;

template<typename Ret, typename Arg> 
struct signature1<Ret(*)(Arg)> {
	using return_type = Ret;
	using argument_type = Arg;
	using function_type = Ret(*)(Arg);
};

template<typename Function> 
struct signature2;

template<typename Ret, typename T0, typename T1> 
struct signature2<Ret(*)(T0, T1)> {
	using return_type = Ret;
	using argument0_type = T0;
	using argument1_type = T1;
	using function_type = Ret(*)(T0,T1);
};

template<typename Method> 
struct method_signature0;

template<typename T, typename Ret> 
struct method_signature0<Ret(T::*)()> {
	using return_type = Ret;
	using basic_type = T;
	using method_type = Ret(T::*)();
};