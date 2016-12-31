#ifndef GQ_ALLOCATOR_H
#define GQ_ALLOCATOR_H

#include <mem/allocator.h>
#include <defines/size_t.h>
#include <gxx/utility.h>
#include <debug/dprint.h>
#include <utilxx/type_analize.h>

namespace gxx {

class Allocator {
public:
	virtual void* allocate(size_t sz) = 0;
	virtual void* reallocate(void* ptr, size_t sz) = 0;
	virtual void deallocate(void* ptr) = 0;

	template<typename T>
	T* allocateObject() {
		return (T*) allocate(sizeof(T));
	};

	template<typename T>
	void deallocateObject(T* ptr) {
		deallocate(ptr);
	};

	template <typename T, typename ... Args>
	T* constructObject(Args ... args) {
		T* ptr = static_cast<T*>(allocate(sizeof(T)));
		gxx::constructor(ptr, args ...);
		return ptr;
	}

	template <typename T>
	void destructObject(T* ptr) {
		gxx::destructor(ptr);
		deallocate(ptr);
	}

	template<typename T>
	T* allocateArray(size_t sz) {
		return (T*) allocate(sz * sizeof(T));
	};

	template<typename T>
	void deallocateArray(T* ptr) {
		deallocate(ptr);
	};

	template <typename T, typename ... Args>
	T* constructArray(size_t sz, Args ... args) {
		T* ptr = (T*)(allocate(sz * sizeof(T)));
		gxx::array_constructor(ptr, ptr + sz, args ...);
		return ptr;
	}

	template <typename T>
	void destructArray(T* ptr, size_t sz) {
		gxx::array_destructor(ptr, ptr + sz);
		deallocate(ptr);
	}
};

class MallocAllocator : public Allocator {
public:
	void* allocate(size_t sz) {
		return malloc(sz);
	};

	void* reallocate(void* ptr, size_t sz) {
		return realloc(ptr, sz);
	};

	void deallocate(void* ptr) {
		free(ptr);
	};
};

class CtrlMallocAllocator : public Allocator {
public:
	void* allocate(size_t sz) {
		return ctrlmalloc(sz);
	};

	void* reallocate(void* ptr, size_t sz) {
		return ctrlrealloc(ptr, sz);
	};

	void deallocate(void* ptr) {
		ctrlfree(ptr);
	};
};

	extern MallocAllocator mallocAllocator;
	extern CtrlMallocAllocator ctrlmallocAllocator;	
	extern Allocator* defaultAllocator;

class BasicAllocated {
public:
	Allocator* m_alloc;

public:
	void setAllocator(Allocator* newalloc) {
		m_alloc = newalloc;
	};

	BasicAllocated() : m_alloc(gxx::defaultAllocator) {};
	BasicAllocated(Allocator* alloc) : m_alloc(alloc) {};
	BasicAllocated(const BasicAllocated& other) {
		m_alloc = other.m_alloc;
	};
};


};

#endif