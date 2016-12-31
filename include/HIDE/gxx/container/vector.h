#ifndef GENOS_VECTOR_H
#define GENOS_VECTOR_H

#include <gxx/algorithm.h>
#include <gxx/allocator.h>
//#include <gxx/dynarray.h>
#include <utilxx/new.h>
#include <assert.h>
//#include "gxx/to_string.h"

namespace gxx {

template<typename T, typename Allocator = system_allocator<T>>
class vector {
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
	size_type _length;

public:
	vector() : _capasity(0), _length(0), _data(nullptr) {}

	int reserve(size_t sz) {
		if (sz > _capasity) return changeBuffer(sz);
		return 1;
	}

	bool is_valid() {
		return _data != nullptr;
	}

	int changeBuffer (size_t sz) {
		assert(sz > _length);
		auto newdata = alloc.allocate(sz);
		if (!is_valid()) {
			_data = newdata;
			_capasity = sz;
			return 1;
		}

		for(int i = 0; i < gxx::min(sz, _length); ++i) {
			new (newdata + i) value_type(gxx::move(*(_data + i)));
		};
		alloc.deallocate(_data);
		_data = newdata;
		_capasity = sz;
		return 1;
	};

	template <typename ... Args>
	void emplace_back(Args ... args) {
		reserve(_length + 1);
		new(_data + _length) T(args ...);
		_length++; 
	}

	void push_back(const T& ref) {
		reserve(_length + 1);
		new(_data + _length) T(ref);
		_length++; 
	}

	value_type& operator[](size_t num) {
		if (num > _length) reserve(num);
		return *(_data + num);
	}

	value_type * begin()
	{
		return _data; 
	};

	value_type * end()
	{
		return _data + _length;
	};

};

};

#endif