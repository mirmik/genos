
#ifndef GENOS_GSTL_HASSIGN_H
	#define GENOS_GSTL_HASSIGN_H
	
	//Операции присваивания и извлечения, призванные игнорировать систему типов.
	//Применяется в анонимных контейнерах.
	
#include "genos/gstl/utility.h"

	template<typename T1, typename T2>
	void hassign(T1& a, const T2& b){*(typename gxx::remove_reference<T2>::type*)&a = b;};
	
	template<typename T1, typename T2>
	void hassign(T1& a, T2&& b){*(typename gxx::remove_reference<T2>::type*)&a = b;};
	
	template<size_t bias, typename T1, typename T2>
	void hassign_bias(T1& a, const T2& b){*(typename gxx::remove_reference<T2>::type*)((char*)&a + bias) = b;};
	
	template<size_t bias, typename T1, typename T2>
	void hassign_bias(T1& a, T2&& b){*(typename gxx::remove_reference<T2>::type*)((char*)&a + bias) = b;};
	
	template<typename T1, typename T2>
	static inline T1& htreat(const T2& a){return *(T1*)&a;};
	
	template<typename T1, typename T2>
	static inline T1& htreat(T2&& a){return *(T1*)&a;};
	
	template<typename T1, size_t bias, typename T2>
	static inline T1& htreat_bias(const T2& a){return *(T1*)((char*)&a + bias);};
		
	template<typename T1, size_t bias, typename T2>
	static inline T1& htreat_bias(T2&& a){return *(T1*)((char*)&a + bias);};

	
#endif