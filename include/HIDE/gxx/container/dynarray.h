#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

//#include "genos/datastruct/dynbuf.h"
//#include "mem/allocator.h"
#include "defines/size_t.h"


namespace gxx {

template<typename T, typename Allocator = system_allocator<listrecord<T>>>
class dynarray {
public:
	using traits_type = elem_traits<T>;	
	using pointer = 		typename traits_type::ptr_type;
	using const_pointer = 	typename traits_type::const_ptr_type;
	using reference = 		typename traits_type::reference;
	using const_reference = typename traits_type::const_reference;
	using size_type = 		typename traits_type::size_type;
	using difference_type = typename traits_type::ptrdiff_t;
	using value_type = 		typename traits_type::value_type;

private:
	Allocator alloc;
	value_type* _data;
	size_type _capasity;

public:
	dynarray() : _capasity(0), _data(nullptr) {}

	int reserve(size_t sz) {
		if (sz > _capasity) return changeBuffer(sz);
		else return 1;
	}

	int changeBuffer (size_t sz) {

	};
};

};
#endif