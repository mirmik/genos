#ifndef GXX_BYTE_RING_H
#define GXX_BYTE_RING_H

#include <inttypes.h>
#include <gxx/Allocator.h>

namespace gxx {
	
	class ByteRing : public BasicAllocated {

		char* m_data;
		
		size_t m_size;
		size_t m_head;
		size_t m_tail;

		inline void fixup(size_t& ref) {
			while (ref >= m_size) ref -= m_size; 
		}

	public:
		ByteRing() : m_data(nullptr), m_size(0), m_head(0), m_tail(0), BasicAllocated() {}

		void reserve(size_t sz) {
			m_data = (char*) m_alloc->reallocate(m_data, sz);
			m_size = sz;
		}

		void invalidate() {
			if (m_data) m_alloc->deallocate(m_data);
			m_data = nullptr;
			m_size = 0;
			m_head = 0;
			m_tail = 0;
		}

		bool is_empty() {
			return m_head == m_tail;
		}

		bool is_full() {
			return m_head == (m_tail ? m_tail : m_size) - 1;
		}

		int putc(char c) {
			if (is_full()) return -1;
			*(m_data + m_head++) = c;
			fixup(m_head);
			return 1;
		}
	
		int getc() {
			if (is_empty()) return -1;
			char c = *(m_data + m_tail++);
			fixup(m_tail);
			return c;
		}

		size_t avail() { 
			return (m_head >= m_tail) ? m_head - m_tail : m_size + m_head - m_tail; 
		};

		size_t room() {
			return (m_head >= m_tail) ? m_size - 1 + ( m_tail - m_head ) : ( m_tail - m_head ) - 1;
		};

		size_t size() {
			return m_size;
		};
	};

};

#endif