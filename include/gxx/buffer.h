#ifndef GENOS_UTILXX_BUFFER_H
#define GENOS_UTILXX_BUFFER_H

#include "stdint.h"
#include "defines/size_t.h"
#include <string.h>
//#include <assert.h>

#include <gxx/array.h>
#include <utilxx//setget.h>

namespace gxx {

	class buffer {
	public:
		char* m_data;
		size_t m_size;
	
	public:
		SETGET(data, m_data);
		SETGET(size, m_size);
		GETTER(bytesize, m_size);

		buffer() : m_data(nullptr), m_size(0) {};
		buffer(const void* __m_data, size_t __m_size) : m_data((char*)(void*)__m_data), m_size(__m_size) {}
		explicit buffer(const char* str)	: m_data((char*)(void*)str), m_size(strlen(str)) {}
	
		template <typename T>
		buffer(T& obj) : m_data((char*)(void*)obj.data()), m_size(obj.bytesize()) {}

		void fill(char chr) {
			memset(m_data, chr, m_size);
		}

		char* begin() {
			return m_data;
		}

		char* end() {
			return m_data + m_size;
		}
	};
	
	void buffer_fill(const buffer& buffer, char c);


	template<typename T> 
	class objbuf {
		T* m_data;
		size_t m_size;
	public:
		SETGET(data, m_data);
		SETGET(size, m_size);
		CONST_GETTER(cdata, m_data);
		CONST_GETTER(csize, m_size);
		GETTER(bytesize, m_size * sizeof(T));

		objbuf() : m_data(nullptr), m_size(0) {};
		objbuf(T* __m_data, size_t __m_size) : m_data(__m_data), m_size(__m_size) {}
		
		T* begin() {
			return m_data;
		}

		T* end() {
			return m_data + m_size;
		}

		using iterator = T*;
	};

}

#endif 