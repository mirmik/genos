#ifndef GENOS_UTILXX_STUB_H
	#define GENOS_UTILXX_STUB_H
	
	
	
	
	template <typename R,typename ... V> R do_nothing(V...){};
	template <typename R,typename ... V> R error_stub(V...){kernel_panic("error_stub");};
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
#endif