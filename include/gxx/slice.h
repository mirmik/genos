#ifndef GXX_SLICE_H
#define GXX_SLICE_H

#include <inttypes.h>
#include <utilxx/setget.h>

namespace gxx {
	template <typename T>
	class slice_type {
	protected:
		T* m_data;
		size_t m_size;

	public:
		ACCESSOR(data, m_data);
		ACCESSOR(size, m_size);
		
		VALUE_GETTER(bytesize, m_size * sizeof(T));

		slice_type() : m_data(nullptr), m_size(0) {};
		slice_type(T* data, size_t size) : m_data(data), m_size(size) {}

		using iterator = T*;

		iterator begin() {
			return m_data;
		}

		iterator end() {
			return m_data + m_size;
		}

		gxx::slice_type<T> slice(){
			return gxx::slice_type<T>(m_data, m_size);
		}

		gxx::slice_type<T> slice(size_t len){
			return gxx::slice_type<T>(m_data, len);
		}

		gxx::slice_type<T> slice(size_t first, size_t len){
			return gxx::slice_type<T>(m_data + first, len);
		}
	};

	//Это хак для того, чтобы определить в классе slice метод slice.
	template <typename T>
	using slice = slice_type<T>;
}

#endif