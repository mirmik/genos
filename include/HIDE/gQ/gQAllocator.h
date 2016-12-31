#ifndef GQ_ALLOCATOR_H
#define GQ_ALLOCATOR_H

#include <mem/allocator.h>
#include <defines/size_t.h>
#include <gxx/utility.h>

class gQAllocator {
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
		gxx::constructor(ptr, ptr + sz, args ...);
		return ptr;
	}

	template <typename T>
	void destructArray(T* ptr, size_t sz) {
		gxx::destructor(ptr, ptr + sz);
		deallocate(ptr);
	}
};

class gQMallocAllocator : public gQAllocator {
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

class gQCtrlMallocAllocator : public gQAllocator {
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

namespace gQ {
	extern gQMallocAllocator gQmalloc;
	extern gQCtrlMallocAllocator gQctrlmalloc;	
};

#endif