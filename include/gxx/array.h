#ifndef GXX_ARRAY_H
#define GXX_ARRAY_H

#include <stdlib.h>
#include <utilxx/setget.h>

namespace gxx {
	template<typename T, size_t Size>
	class array {
		T m_array[Size];
		
	public:
		CONSTREF_GETTER(data, m_array);
		
		VALUE_GETTER(bytesize, Size * sizeof(T));
		VALUE_GETTER(size, Size);

		operator gxx::slice<T>() { return slice(); }

		gxx::slice<T> slice(){
			return gxx::slice<T>(m_array, Size);
		}

		gxx::slice<T> slice(size_t len){
			return gxx::slice<T>(m_array, len);
		}

		gxx::slice<T> slice(size_t first, size_t len){
			return gxx::slice<T>(m_array + first, len);
		}
	};
}

#endif