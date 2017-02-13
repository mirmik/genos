#ifndef GENOS_HASH_H
#define GENOS_HASH_H

#include <string.h>
#include <inttypes.h>

#ifdef GENOS_CONFIGURE
#	include "genos_configure.h"
#endif

#ifdef GXX_DEFAULT_SEED
#define GXX_DEFAULT_SEED 0xABCDEFAB
#endif

namespace gxx {

	extern uint32_t defaultSeed; 

	template <typename T> 
	inline size_t hash(const T& other, uint32_t seed) {
		return other.hash(seed);
	}

	inline size_t hash(const char * p, size_t len, uint32_t seed) {
		size_t h = seed;
		for (int i = 0; i < len; ++i)
			h = 31 * h + p[i];
		return h;
	}

	inline size_t hash(int8_t val, uint32_t seed) { return val ^ seed;}
	inline size_t hash(int16_t val, uint32_t seed) { return val ^ seed; }
	inline size_t hash(int32_t val, uint32_t seed) { return val ^ seed; }
	inline size_t hash(int64_t val, uint32_t seed) { return val ^ seed; }
	inline size_t hash(uint8_t val, uint32_t seed) { return val ^ seed; }
	inline size_t hash(uint16_t val, uint32_t seed) { return val ^ seed; }
	inline size_t hash(uint32_t val, uint32_t seed) { return val ^ seed; }
	inline size_t hash(uint64_t val, uint32_t seed) { return val ^ seed; }

	inline size_t hash(const char* val, uint32_t seed) { return hash(val,strlen(val),seed); }
};

#endif