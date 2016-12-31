

template<typename Function> 
struct signature;

template<typename Ret, typename Arg> 
struct signature<Ret(*)(Arg)> {
	using ret_type = Ret;
	using arg_type = Arg;
	using func_type = Ret(*)(Arg);
};

template<typename Function> 
struct signature2;

template<typename Ret, typename T0, typename T1> 
struct signature2<Ret(*)(T0, T1)> {
	using ret_type = Ret;
	using t0 = T0;
	using t1 = T1;
	using func_type = Ret(*)(T0,T1);
};