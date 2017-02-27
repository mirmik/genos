
#ifndef GENOS_GSTL_UTILITY
#define GENOS_GSTL_UTILITY

//Реализации разбиты по нескольким файлам, дабы не было лапши из кода.
//Некоторые шаблоны могут врать... Извините.
//UNDER_CONSTRUCTED

//#include <genos/gstl/algorithm.h>

#include "defines/nullptr_t.h"
#include "defines/size_t.h"
#include <utilxx/new.h>

#include <debug/dprint.h>
#include <utilxx/type_analize.h>

namespace gxx{
	using size_t = ::size_t;
	
	#include "gxx/utility/prototype.hpp"
	
	#include "gxx/utility/pair.hpp"
	#include "gxx/utility/utility.hpp"
	#include "gxx/utility/function_parse.hpp"
	#include "gxx/utility/type_traits.hpp"
	#include "gxx/utility/type_transform.hpp"
	#include "gxx/utility/type_relation.hpp"
	#include "gxx/utility/move.hpp"
	
	template<typename T>
	void destructor(T* ptr) {
		ptr->~T();
	}
	
	template<class InputIterator>  
	void
	array_destructor(InputIterator first, InputIterator last)
	{
		while(first != last){
			gxx::destructor(&*first);
			++first;
		}
	}
	
	template<typename T, typename ... Args>
	void constructor(T* ptr, Args&& ... args) {
		new(ptr) T(gxx::forward<Args>(args)...);
	}
	
	template<typename T>
	void copyConstructor(T* ptr, const T& other) {
		new(ptr) T(other);
	}
	
	template<typename T>
	void moveConstructor(T* ptr, T&& other) {
		new(ptr) T(gxx::forward<T>(other));
	}
	
	template<class InputIterator, typename ... Args>  
	void
	array_constructor(InputIterator first, InputIterator last, Args ... args) {
		while(first != last){
			gxx::constructor(&*first, args ...);
			++first;
		}
	}
	
	template<typename T>
	void delete_expression(T* ptr) {
		delete ptr;
	} 
	
};

#endif