#ifndef GENOS_GQ_LIST_H
#define GENOS_GQ_LIST_H

#include <gQ/gQByteAllocated.h>
#include <gQ/gQTraits.h>
#include <gxx/utility.h>
#include <gxx/algorithm.h>
#include <gxx/debug/dprint.h>

#include <string.h>
#include <assert.h>

template<typename T>
class gQList : public gQByteAllocated {
	using val_t = typename gQTraits<T>::val_t;
	using ref_t = typename gQTraits<T>::ref_t;
	using rref_t = typename gQTraits<T>::rref_t;
	using ptr_t = typename gQTraits<T>::ptr_t;
	using size_t = typename gQTraits<T>::size_t;
	using diff_t = typename gQTraits<T>::diff_t;

private:
	ptr_t* m_array;
	size_t m_capasity;
	size_t m_size;

public:	
	gQList() : m_array(nullptr), m_capasity(0), m_size(0), gQByteAllocated() {};

private:
	void invalidate() {
		if (m_array) {
			m_alloc->deallocate((void*)m_array);
		};
		m_capasity = 0;
		m_size = 0;
	}

	int changeBuffer(size_t sz) {
		m_array = (ptr_t*)m_alloc->reallocate(m_array, sz * sizeof(ptr_t));
		m_capasity = sz;
		return 0;
	};

	int reserve(size_t sz) {
		if (sz <= m_capasity) return 0;
		size_t need = m_capasity ? m_capasity : 1;
		while(need < sz) need = need * 2;
		return changeBuffer(need);
	};

	void shift(size_t soff, size_t offset) {
		assert(soff <= m_size);

		ptr_t* dst;
		if (m_capasity < m_size + offset)
			dst = (ptr_t*) m_alloc->allocate((m_size + offset) * sizeof(ptr_t));
		else dst = m_array;

		memmove(dst + soff + offset, m_array + soff, (m_size - soff) * sizeof(ptr_t));

		if (dst != m_array) {
			memmove(dst, m_array, soff * sizeof(ptr_t));
			auto temp = m_array;
			m_array = dst;
			m_alloc->deallocate(temp);
		} 
	};

public:
	void push_back(const T& ref) {
		reserve(m_size + 1);
		ptr_t obj = new(m_alloc) T(ref);
		*(m_array + m_size) = obj;
		++m_size;
	}

	void push_front(const T& ref) {
		shift(0,1);
		ptr_t obj = new(m_alloc) T(ref);
		*(m_array) = obj;
		++m_size;
	}

	void pop_front() {
		assert(m_size > 0);
		gxx::destructor(*m_array);
		m_alloc->deallocate((void*)*m_array);
		shift(1,-1);
		--m_size;
	};

	size_t size() {
		return m_size;
	}

	class iterator {
		ptr_t* ptr;
	public:
		iterator(ptr_t* _ptr) : ptr(_ptr) {};

		iterator operator++(int) { iterator i = *this; ++ptr; return i; }
		iterator operator++() { ++ptr; return *this; }
		bool operator!= (const iterator& b) {return ptr != b.ptr;};
		bool operator== (const iterator& b) {return ptr == b.ptr;};
		val_t & operator*() {return **ptr;};
		val_t * operator->() {return *ptr;};
	};

	iterator begin() {return iterator(m_array);}
	iterator end() {return iterator(m_array + m_size);}
};

#endif 