#ifndef GENOS_UTILXX_BUFFER_H
#define GENOS_UTILXX_BUFFER_H

#include "stdint.h"
#include "defines/size_t.h"
#include <string.h>
//#include <assert.h>

namespace gxx {

	class buffer {
	public:
		void* m_data;
		size_t m_size;
	
	public:
		buffer(const void* __data, size_t __size) : m_data((void*)__data), m_size(__size) {};
		explicit buffer(const char* str)	: m_data((void*)str), m_size(strlen(str)) {};
	
		void* data() const { return m_data; };
		size_t size() const { return m_size; };

		void fill(char chr) {
			memset(m_data, chr, m_size);
		}
	};
	
	void buffer_fill(const buffer& buffer, char c);

};

#endif 