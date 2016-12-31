#ifndef GENOS_VECTOR_H
#define GENOS_VECTOR_H

#include <gxx/utility.h>
#include <gxx/algorithm.h>
#include <gxx/Allocator.h>

namespace gxx {

	template<typename T>
	class Vector : public BasicAllocated {
	private:
		T* m_data;
		size_t m_capasity;
		size_t m_size;
	
	public:
		Vector() : m_capasity(0), m_size(0), m_data(nullptr) {}
	
		~Vector() {
			gxx::array_destructor(begin(),end());
			if (m_data) m_alloc->deallocateArray(m_data);
		}

		unsigned char reserve(size_t sz) {
			if (sz > m_capasity) return changeBuffer(sz);
			return 1;
		}
	
		unsigned char changeBuffer (size_t sz) {
			auto newbuf = m_alloc->allocateArray<T>(sz);
			m_capasity = sz;
			if (m_data == nullptr) {
				m_data = newbuf;
				return 1;
			}
			auto itpair = gxx::save_move(begin(), end(), newbuf, newbuf + sz);
			gxx::array_destructor(itpair.first, end());
			auto oldbuf = m_data; 
			m_data = newbuf;
			m_alloc->deallocateArray<T>(oldbuf);
			return 1;
		};

		T* begin() {
			return m_data;
		}

		T* end() {
			return m_data + m_size;
		}
	
		template <typename ... Args>
		void emplace_back(Args ... args) {
			reserve(m_size + 1);
			gxx::constructor(m_data + m_size, args ...);
			m_size++; 
		}
	
		void push_back(const T& ref) {
			reserve(m_size + 1);
			gxx::constructor(m_data + m_size, ref);
			m_size++; 
		}
	
		T& operator[](size_t num) {
			reserve(num + 1);
			if (num >= m_size) m_size = num + 1;
			return *(m_data + num);
		}

		size_t size() {
			return m_size;
		}

		size_t capacity() {
			return m_capasity;
		}
	/*
		value_type * begin()
		{
			return _data; 
		};
	
		value_type * end()
		{
			return _data + _length;
		};
	*/
	};
};

#endif