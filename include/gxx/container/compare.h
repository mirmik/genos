#ifndef GXX_COMPARE_H
#define GXX_COMPARE_H

#include <string.h>

namespace gxx {

	template<typename T>
	inline bool compare(const T& a, const T& b) { return a == b; }

	inline bool compare(const char* a, const char* b) {
		return !strcmp(a,b);
	}


	template<typename T>
	inline bool equal(const T& a, const T& b) { return a == b; }

	inline bool equal(const char* a, const char* b) {
		return !strcmp(a,b);
	}

}

#endif