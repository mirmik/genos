#ifndef GXX_COMPARE_H
#define GXX_COMPARE_H

#include <string.h>

namespace gxx {

	template<typename T>
	inline bool compare(T a, T b) { return a == b; }

	inline bool compare(const char* a, const char* b) {
		return !strcmp(a,b);
	}

}

#endif