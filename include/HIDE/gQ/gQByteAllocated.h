#ifndef GENOS_GQ_BYTE_ALLOCATED_H
#define GENOS_GQ_BYTE_ALLOCATED_H

#include <gQ/gQAllocator.h>
#include <gQ/gQBase.h>
#include <gQ/util/gQMemoryStrategy.h>

#include <gxx/utility.h>
#include <gxx/algorithm.h>

class gQBasicAllocated {
protected:
	gQAllocator* m_alloc;

public:
	void setAllocator(gQAllocator* newalloc) {
		m_alloc = newalloc;
	};

	gQBasicAllocated() : m_alloc(gQ::defaultAllocator) {};
	gQBasicAllocated(const gQBasicAllocated& other) {
		m_alloc = other.m_alloc;
	};
};

/*
template <typename T> 
class gQAllocatedArray : public gQBasicAllocated {
	T* m_data;
	size_t m_capacity;
	size_t m_size;
	size_t (*m_memoryStrategy)(size_t);

public:
	gQAllocatedArray() : m_data(nullptr), m_capacity(0), m_size(0),
		m_memoryStrategy(naiveMemoryStrategy) {};

	void invalidate(void) {
		gxx::destructor(m_data, m_data + m_size);
		if (m_data) m_alloc->deallocate(m_data);
		m_data = NULL;
		m_capacity = m_size = 0;
	}

	unsigned char reserve(size_t newsize) {
		if (m_data && m_capacity >= m_size) return 1;
		if (changeBuffer(m_memoryStrategy(newsize))) {
			return 1;
		}
		return 0;
	}
	
	unsigned char resize(size_t newsize) {
		reserve(newsize);
		m_size = newsize;
		return 1;
	}

	unsigned char changeBuffer(size_t newsize) {
		T* newdata = (T*) m_alloc->allocate(newsize * sizeof(T));
		if (m_data == nullptr) {
			m_data = newdata;
			return 1;
		}
		gxx::move(m_data, m_data + m_size, newdata);
		gxx::swap(m_data, newdata);
		m_alloc->deallocate(newdata);
		return 1;
	}
};*/

#endif