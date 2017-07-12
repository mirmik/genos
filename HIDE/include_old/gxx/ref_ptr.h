#ifndef GXX_REF_PTR_H
#define GXX_REF_PTR_H

#include <assert.h>

namespace gxx {
	template <typename T, uint8_t T::* ref, typename Deleter>
	class ref_ptr : public Deleter {
		T* ptr;

	public:
		ref_ptr() : ptr(nullptr) {}

		ref_ptr(T* ptr) : ptr(ptr) { 
			//dprln("ref_ptr()");
			ptr->ref++; 
		}

		ref_ptr(const ref_ptr& other) : ptr(other.ptr) { 
			//dprln("ref_ptr(const ref_ptr&)");
			ptr->ref++; 
		}

		~ref_ptr() { 
			if (!ptr) return;
			//dprln("~ref_ptr()");
			ptr->ref--; 
			if (ptr->ref == 0) Deleter::deleter(ptr); 
		}

		T* operator-> () {
			assert(ptr);
			return ptr;
		}

		T& operator* () {
			assert(ptr);
			return *ptr;
		}

		T* get() {
			assert(ptr);
			return ptr;
		}

		void reset(T* other_ptr) {
			if (ptr) {
				ptr->ref--; 
				if (ptr->ref == 0) Deleter::deleter(ptr); 
			}

			ptr = other_ptr;
			if (ptr) ptr->ref++; 
		}

		void reset(const ref_ptr& other) {
			if (ptr) {
				ptr->ref--; 
				if (ptr->ref == 0) Deleter::deleter(ptr); 
			}

			ptr = other.ptr;
			if (ptr) ptr->ref++; 
		}
	};
}

#endif