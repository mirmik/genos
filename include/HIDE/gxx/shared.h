#ifndef GXX_SHARED_H
#define GXX_SHARED_H

#include <defines/atomic_t.h>

namespace gxx {

template <typename T> class shared_ptr;

template <typename T>
class shared_counter {
private:
	atomic_t refs;
	T* obj;
public:
	shared_counter(T* _obj) : obj(_obj), refs(1) {};

friend shared_ptr<T>;
};

template <typename T>
class shared_ptr {	
private:
	shared_counter<T>* counter;

public:
	shared_ptr(T* obj) {
		counter = new shared_counter<T>(obj);
	}

	shared_ptr(const shared_ptr& other) {
		counter = other.counter;
		++counter->refs;
	}

	~shared_ptr() {
		atomic_t refs = --counter->refs;
		if (refs == 0) {
			delete counter->obj;
			delete counter;
		}
	}

	T& operator*() {
		return *counter->obj;
	};
};

};

#endif