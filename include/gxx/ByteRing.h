#ifndef GXX_BYTE_RING_H
#define GXX_BYTE_RING_H

#include <inttypes.h>
#include <gxx/Allocator.h>

namespace gxx {
	
	class ByteRing : public BasicAllocated {

		char* m_data;
		
		size_t size;
		size_t head;
		size_t tail;

		inline void fixup(size_t& ref) {
			while (ref >= size) ref -= size; 
		}

	public:
		ByteRing() : m_data(nullptr), size(0), head(0), tail(0), BasicAllocated() {}

		void reserve(size_t sz) {
			m_data = (char*) m_alloc->reallocate(m_data, sz);
			size = sz;
		}

		void invalidate() {
			if (m_data) m_alloc->deallocate(m_data);
			m_data = nullptr;
			size = 0;
			head = 0;
			tail = 0;
		}

		bool is_empty() {
			return head == tail;
		}

		bool is_full() {
			return head == (tail ? tail : size) - 1;
		}

		int putc(char c) {
			if (is_full()) return -1;
			*(m_data + head++) = c;
			fixup(head);
			return 1;
		}
	
		int getc() {
			if (is_empty()) return -1;
			char c = *(m_data + tail++);
			fixup(tail);
			return c;
		}

		size_t avail() { 
			return (head >= tail) ? head - tail : size + head - tail; 
		};

		size_t room() {
			return (head >= tail) ? size - 1 + ( tail - head ) : ( tail - head ) - 1;
		};

	};

};

#endif