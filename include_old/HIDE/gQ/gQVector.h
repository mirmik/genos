#ifndef GENOS_GQ_VECTOR_H
#define GENOS_GQ_VECTOR_H

#include <gQ/gQByteAllocated.h>
#include <gQ/gQTraits.h>
#include <gxx/utility.h>
#include <gxx/algorithm.h>
#include <debug/dprint.h>

#include <string.h>
#include <assert.h>

template<typename T>
class gQVector : public gQByteAllocated {
	using val_t = typename gQTraits<T>::val_t;
	using ref_t = typename gQTraits<T>::ref_t;
	using rref_t = typename gQTraits<T>::rref_t;
	using ptr_t = typename gQTraits<T>::ptr_t;
	using size_t = typename gQTraits<T>::size_t;
	using diff_t = typename gQTraits<T>::diff_t;

	ptr_t m_array;
	size_t m_capasity;
	size_t m_size;

public:	
	gQVector() : m_array(nullptr), m_capasity(0), m_size(0), gQByteAllocated() {};

private:
	void invalidate() {
		if (m_array) {
			m_alloc->deallocate((void*)m_array);
		};
		m_capasity = 0;
		m_size = 0;
	}

	int changeBuffer(size_t sz) {
		ptr_t newarray = (ptr_t) m_alloc->allocate(sz * sizeof(val_t));
		
		if (m_array == nullptr) {
			m_array = newarray;
			return 0;
		}

		for(ptr_t src = m_array, dst = newarray; src < m_array + m_size;) {
			*dst++ = gxx::move(*src++);
		}
		auto temp = m_array;
		m_array = newarray;
		m_alloc->deallocate(temp);
		m_capasity = sz;
		return 0;
	};

	size_t memoryStrategy(size_t need) {
		size_t ret = m_capasity ? m_capasity : 1;
		while(ret < need) ret *= 2;
		return ret;
	};

	int reserve(size_t sz) {
		if (sz <= m_capasity) return 0;
		size_t need = memoryStrategy(sz);
		return changeBuffer(need);
	};

	T* reserveWithoutCopy(size_t sz) {
		if (sz <= m_capasity) return m_array;
		m_capasity = memoryStrategy(sz);
		auto ret = (ptr_t) m_alloc->allocate(m_capasity * sizeof(val_t));		
		return ret;
	}

	void shift(size_t soff, size_t offset) {
		assert(soff <= m_size);

		ptr_t dst;
		if (m_capasity < m_size + offset)
			dst = reserveWithoutCopy(m_size + offset);
		else dst = m_array;

		gxx::objmove(dst + soff + offset, m_array + soff, m_size - soff);

		if (dst != m_array) {
			gxx::objmove(dst, m_array, soff);
			auto temp = m_array;
			m_array = dst;
			m_alloc->deallocate(temp);
		} 
	};

	/*
	ptr_t createNewBufferMove(size_t ncap, size_t soff, size_t off) {
		assert(ncap >= off + m_size);
		assert(soff + off >= 0);
		ptr_t newbuf = (ptr_t) m_alloc->allocate(ncap * sizeof(T));
		gxx::objmove(newbuf + soff + off, m_array + soff, m_size - soff);
		if (off < 0) gxx::objmove(newbuf, m_array, soff + off);
		else gxx::objmove(newbuf, m_array, soff);
	}*/

	//void copyImplementation() {
	//};

public:
	void push_back(const T& ref) {
		reserve(m_size + 1);
		new(m_array + m_size) T(ref);
		++m_size;
	}

	void push_front(const T& ref) {
		shift(0,1);
		new(m_array) T(ref);
		++m_size;
	}

	using iterator = ptr_t;

	iterator begin() {
		return m_array;
	};

	iterator end() {
		return m_array + m_size;
	};
};

#endif