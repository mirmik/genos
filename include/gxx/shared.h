#ifndef GXX_SHARED_H
#define GXX_SHARED_H

#include <gxx/allocator.h>

namespace gxx {


	template <typename T> class shared_ptr;

	template <typename T, class Allocator = gxx::allocator<T>>
	class shared_control_block {
		uint16_t ref;
		friend class shared_ptr<T>;
	};
	
	template <typename T>
	class shared_ptr {
		T * ptr; 

		explicit shared_ptr(T* ptr) : ptr(ptr) {}

	public:
		T* get() { return ptr; };
	};

}

#endif