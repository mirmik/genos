#ifndef GENOS_CONTAINER_TRAITS_H
#define GENOS_CONTAINER_TRAITS_H

#include "string.h"

static uint32_t strhash(const char* str) {
	uint32_t sum = 0;
	uint8_t c;
	while(c = *str++) sum += c;
	return sum; 
}

template <typename type> struct traits {};

template<> struct traits<const char*> {
	using hash_fn_t = uint32_t(*)(const char*);
	using cmp_fn_t = int32_t(*)(const char*,const char*);
	static constexpr hash_fn_t hash_fn = &strhash;
	static constexpr cmp_fn_t cmp_fn = &strcmp;	
};

#endif