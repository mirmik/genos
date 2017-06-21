#ifndef GXX_COMMUNICATION_BUFFER_H
#define GXX_COMMUNICATION_BUFFER_H

#include <gxx/Allocator.h>
#include <genos_configure.h>
#include <string.h>
#include <assert.h>
#include <gxx/buffer.h>

namespace gxx {
	class CommunicationBuffer : public BasicAllocated {
	private:
		char* m_data;
		
		size_t m_capacity;
		size_t m_head;
		size_t m_tail;

	public:
		CommunicationBuffer(Allocator* alloc = GXX_DEFAULT_ALLOCATOR) : BasicAllocated(alloc) 
		,	m_data(nullptr), m_tail(0), m_head(0), m_capacity(0) {}


		CommunicationBuffer(size_t cap, size_t prefix, Allocator* alloc = GXX_DEFAULT_ALLOCATOR) : BasicAllocated(alloc)
		, m_capacity(cap), m_head(prefix), m_tail(prefix) {
			assert(prefix < cap);
			m_data = (char*) m_alloc->allocate(cap);
			
			m_capacity = cap;
			m_head = prefix;
			m_tail = prefix;
		}

		CommunicationBuffer(const CommunicationBuffer&) = delete;

		ssize_t concat_back(const char* data, size_t length) {
			if (m_capacity - m_tail < length) return -1;
			memcpy(m_data + m_tail, data, length);
			m_tail += length;
			return length;
		}

		template<typename Buffer>
		ssize_t concat_back(const Buffer & buffer) {
			size_t length = buffer.size();
			if (m_capacity - m_tail < length) return -1;
			memcpy(m_data + m_tail, buffer.data(), length);
			m_tail += length;
			return length;
		}

		ssize_t concat_back(const char chr) {
			if (m_capacity - m_tail < 1) return -1;
			m_data[m_tail] = chr;
			m_tail += 1;
			return 1;
		}

		ssize_t concat_back(const char* str) {
			return concat_back(str, strlen(str));
		}

		ssize_t concat_front(const char* data, size_t length) {
			if (m_head < length) return -1;
			memcpy(m_data + m_head - length, data, length);
			m_head -= length;
			return length;
		}

		template<typename Buffer>
		ssize_t concat_front(const Buffer & buffer) {
			size_t length = buffer.size();
			if (m_head < length) return -1;
			memcpy(m_data + m_head - length, buffer.data(), length);
			m_head -= length;
			return length;
		}
	
		ssize_t concat_front(const char chr) {
			if (m_head < 1) return -1;
			m_data[m_head - 1] = chr;
			m_head -= 1;
			return 1;
		}

		ssize_t concat_front(const char* str) {
			return concat_front(str, strlen(str));
		}

		gxx::buffer toBuffer() {
			return gxx::buffer(m_data + m_head, m_tail - m_head);
		}

		void* data() const {
			return m_data + m_head;
		}

		size_t size() const {
			return m_tail - m_head;
		}

		~CommunicationBuffer() {
			m_alloc->deallocate(m_data);
		}
	};
}

#endif