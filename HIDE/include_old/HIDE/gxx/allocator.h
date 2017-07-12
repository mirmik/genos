#ifndef GENOS_GSTL_ALLOCATOR
#define GENOS_GSTL_ALLOCATOR

#include <mem/allocator.h>
#include "genos_configure.h"	
#include <gxx/debug/dprint.h>
#include "gxx/traits.h"

namespace gxx {

template <typename T, typename traits_type = elem_traits<T>>
class allocator { 
	public:
	typedef typename traits_type::	ptr_type 			pointer;
	typedef typename traits_type::	size_type 			size_type;
	
	virtual pointer allocate(size_type n) = 0;
	virtual pointer reallocate(pointer old, size_type n) = 0;
	virtual void deallocate(pointer p) = 0;
};

template <typename T, typename traits_type = elem_traits<T>>
class system_allocator : public allocator<T,traits_type> { 
	public:
	typedef typename traits_type::	ptr_type 			pointer;
	typedef typename traits_type::	const_ptr_type 		const_pointer;
	typedef typename traits_type::	reference 			reference;
	typedef typename traits_type::	const_reference 	const_reference;
	typedef 						T 					value_type;
	typedef typename traits_type::	size_type 			size_type;
	typedef typename traits_type::	ptrdiff_t 			difference_type;
	
	public:
	pointer allocate(size_type n) { 
		return reinterpret_cast<pointer>(sysalloc(n * sizeof(T))); 
	};
	
	pointer reallocate(pointer old, size_type n) { 
		return reinterpret_cast<pointer>(sysrealloc(old, n * sizeof(T))); 
	};

	void deallocate(pointer p) {
		sysfree(reinterpret_cast<void*>(p)); 
	};
};	
/*template <typename T, typename traits_type = elem_traits<T>>
class malloc_allocator : public allocator<T,traits_type> { 
	using pointer = typename allocator<T,traits_type>::pointer;
	using size_type = typename allocator<T,traits_type>::size_type;
	
	public:
	pointer allocate(size_type n) { 
		return reinterpret_cast<pointer>(malloc(n * sizeof(T))); 
	};
	
	pointer reallocate(pointer old, size_type n) { 
		return reinterpret_cast<pointer>(realloc(old, n * sizeof(T))); 
	};

	void deallocate(pointer p) {
		free(reinterpret_cast<void*>(p)); 
	};
};	

static void* ctrlmalloc (size_t sz) {
	void * ret = malloc(sz);
	debug_print("MEMORY malloc\t");debug_printdec_uint32(sz);
	debug_print("\t\tret ");debug_printhex_ptr(ret);  dln(); 
	return ret;
};

static void ctrlfree (void* ptr) {
	free(ptr);
	debug_print("MEMORY free\t"); debug_printhex_ptr(ptr); dln();
};

static void* ctrlrealloc (void* ptr, size_t sz) {
	void * ret = realloc(ptr,sz);
	debug_print("MEMORY realloc\t");debug_printdec_uint32(sz); 
	debug_print(", ");debug_printhex_ptr(ptr);
	debug_print("\tret ");debug_printhex_ptr(ret);  dln(); 
	return ret;
};

template <typename T, typename traits_type = elem_traits<T>>
class ctrlmalloc_allocator : public allocator<T,traits_type> { 
	using pointer = typename allocator<T,traits_type>::pointer;
	using size_type = typename allocator<T,traits_type>::size_type;
	
	public:
	pointer allocate(size_type n) { 
		return reinterpret_cast<pointer>(ctrlmalloc(n * sizeof(T))); 
	};
	
	pointer reallocate(pointer old, size_type n) { 
		return reinterpret_cast<pointer>(ctrlrealloc(old, n * sizeof(T))); 
	};

	void deallocate(pointer p) {
		ctrlfree(reinterpret_cast<void*>(p)); 
	};
};	*/

};	
	
#endif