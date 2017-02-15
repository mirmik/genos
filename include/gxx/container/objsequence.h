#ifndef GXX_CONTAINER_SEQUENCE_H
#define GXX_CONTAINER_SEQUENCE_H

#include <gxx/algorithm.h>
#include <gxx/allocator.h>

namespace gxx {

	template<typename T, class Allocator = gxx::allocator<T>>
	class objsequence {
	public: 
		//types
		using iterator = T*;

	protected:
		//resources
		T* m_data = nullptr;
		size_t m_size = 0;
		size_t m_capacity = 0;

		//generated
		Allocator 	m_alloc;

	public:
		
		//constructors
		objsequence( const Allocator& alloc = Allocator() ) : m_alloc(alloc) {}

		//destructors
		~objsequence() {
			if (m_data) m_alloc.deallocate(m_data);
		}

		//methods
		unsigned char reserve(size_t sz) {
			if (sz > m_capacity) return changeBuffer(sz);
			return 1;
		}

		iterator begin() {return m_data;}
		iterator end() {return m_data + m_size;}

	private:
		unsigned char changeBuffer (size_t sz) {
			auto newbuf = m_alloc.allocate(sz);
			m_capacity = sz;

			if (m_data == nullptr) {
				m_data = newbuf;
				return 1;
			}

			//Т.к. функция changeBuffer может вызываться для сокращения массива,
			//вызываем функцию перемещения, которая контролирует размер массива приёмника. 
			auto itpair = gxx::save_move(begin(), end(), newbuf, newbuf + sz);

			//Для неперемещенных элементов вызываем деструкторы.
			gxx::array_destructor(itpair.first, end());
			
			auto oldbuf = m_data; 
			m_data = newbuf;
			m_alloc.deallocate(oldbuf);
			
			return 1;
		}
	};
}

#endif