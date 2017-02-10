#ifndef GXX_ARRAY_H
#define GXX_ARRAY_H

#include <stdlib.h>
#include <utilxx/setget.h>

namespace gxx {
	template<typename T, size_t Size>
	class array {
		T m_array[Size];
		
	public:
		GETTER(data, &m_array[0]);
		GETTER(bytesize, Size * sizeof(T));
		GETTER(size, Size);
	};
}

#endif