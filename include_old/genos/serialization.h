#ifndef GENOS_SERIALIZATION_2_H
#define GENOS_SERIALIZATION_2_H

#include <debug/dprint.h>
#include "genos_configure.h"
#include <defines/size_t.h>
#include <hal/arch.h>
#include <inttypes.h>
#include <string.h>

#define PROTOORD_LITTLE 4
#define PROTOORD_BIG 8
#define PROTOORD_MASK 12

static bool is_need_reverse(uint8_t order) {
#if BYTE_ORDER == __LITTLE_ENDIAN
	if (order == PROTOORD_BIG) return true;
	return false;
#elif BYTE_ORDER == __BIG_ENDIAN
	if (order == PROTOORD_LITTLE) return true;
	return false;
#else
	#error "Architecture error!"
#endif
};

static void memcpy_swap(void* dst, const void* src, size_t sz) {
	//dprln("here");
	char* _dst = (char*) dst + sz -1;
	char* _src = (char*) src;
	char* _end = _src + sz;

	while(_src != _end) {
		*_dst-- = *_src++; 
	}
};

static inline void __memcpy(void* dst, const void* src, size_t sz, uint8_t flag) {
	if (is_need_reverse(flag)) memcpy_swap(dst,src,sz);
	else memcpy(dst,src,sz);
}


template <typename type>
unsigned int serialization(char* buf, type& obj, uint8_t flag) {
	obj.serialization(buf,flag);
};

//serialization template
template <typename T>
size_t serialization(char* ptr, const T& ref, size_t maxlen, uint8_t flag) {
	return ref.serialization(ptr, maxlen, flag);
}

//deserialization template
template <typename T>
size_t deserialization(char* ptr, T& ref, size_t maxlen, uint8_t flag) {
	return ref.deserialization(ptr, maxlen, flag);
}

//serialsize template
template <typename T>
size_t serialSize(const T& ref) {
	return ref.serialSize();
}

//serialsize template
template <typename T> size_t deserialSize(char* ptr) {
	return deserialSize<T>(ptr);
};

template <> size_t deserialSize<int8_t>(char* ptr); 
template <> size_t deserialSize<int16_t>(char* ptr);
template <> size_t deserialSize<int32_t>(char* ptr);
template <> size_t deserialSize<int64_t>(char* ptr);

template <> size_t deserialSize<uint8_t>(char* ptr); 
template <> size_t deserialSize<uint16_t>(char* ptr);
template <> size_t deserialSize<uint32_t>(char* ptr);
template <> size_t deserialSize<uint64_t>(char* ptr);

//unsigned integer type serialization
static inline size_t serialization(char* ptr, const uint8_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 1) return 0; __memcpy(ptr, &ref, 1, flag); return 1;
};

static inline size_t serialization(char* ptr, const uint16_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 2) return 0; __memcpy(ptr, &ref, 2, flag); return 2;
};

static inline size_t serialization(char* ptr, const uint32_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 4) return 0; __memcpy(ptr, &ref, 4, flag); return 4;
};

static inline size_t serialization(char* ptr, const uint64_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 8) return 0; __memcpy(ptr, &ref, 8, flag); return 8;
};

//signed integer type serialization
static inline size_t serialization(char* ptr, const int8_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 1) return 0; __memcpy(ptr, &ref, 1, flag); return 1;
};

static inline size_t serialization(char* ptr, const int16_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 2) return 0; __memcpy(ptr, &ref, 2, flag); return 2;
};

static inline size_t serialization(char* ptr, const int32_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 4) return 0; __memcpy(ptr, &ref, 4, flag); return 4; 
};

static inline size_t serialization(char* ptr, const int64_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 8) return 0; __memcpy(ptr, &ref, 8, flag); return 8;
};

static inline size_t plainSerialization(char* dst, void* src, uint16_t len, size_t maxlen) { 
	if (maxlen < len) return 0;	memcpy(dst, src, len); return len; 
};


//unsigned integer type deserialization
static inline size_t deserialization(char* ptr, uint8_t& ref, size_t maxlen, uint8_t flag) { 
	dprln("deser uint8_t");
	if (maxlen < 1) return 0; __memcpy(&ref, ptr, 1, flag); return 1;
};

static inline size_t deserialization(char* ptr, uint16_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 2) return 0; __memcpy(&ref, ptr, 2, flag); return 2;
};

static inline size_t deserialization(char* ptr, uint32_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 4) return 0; __memcpy(&ref, ptr, 4, flag); return 4;
};

static inline size_t deserialization(char* ptr, uint64_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 8) return 0; __memcpy(&ref, ptr, 8, flag); return 8;
};

//signed integer type deserialization
static inline size_t deserialization(char* ptr, int8_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 1) return 0; __memcpy(&ref, ptr, 1, flag); return 1;
};

static inline size_t deserialization(char* ptr, int16_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 2) return 0; __memcpy(&ref, ptr, 2, flag); return 2;
};

static inline size_t deserialization(char* ptr, int32_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 4) return 0; __memcpy(&ref, ptr, 4, flag); return 4;
};

static inline size_t deserialization(char* ptr, int64_t& ref, size_t maxlen, uint8_t flag) { 
	if (maxlen < 8) return 0; __memcpy(&ref, ptr, 8, flag); return 8;
};

static inline size_t plainDeserialization(char* dst, void* src, uint16_t len, size_t maxlen) { 
	if (maxlen < len) return 0;	memcpy(src, dst, len); return len; 
};

//unsigned integer type serialSize
static inline size_t serialSize(const uint8_t& ref) { return 1; };
static inline size_t serialSize(const uint16_t& ref) { return 2; };
static inline size_t serialSize(const uint32_t& ref) { return 4; };
static inline size_t serialSize(const uint64_t& ref) { return 8; };

//signed integer type serialSize
static inline size_t serialSize(const int8_t& ref) { return 1; };
static inline size_t serialSize(const int16_t& ref) { return 2; };
static inline size_t serialSize(const int32_t& ref) { return 4; };
static inline size_t serialSize(const int64_t& ref) { return 8; };

class DeserialBuffer {
	uint16_t m_size;
	char* m_data;

public:
	DeserialBuffer(char* buf, uint16_t sz) : m_data(buf), m_size(sz) {};

	template <typename T>
	unsigned char deserialization(T& ref, uint8_t flag = GENOS_SERIAL_ORDER) {
		if (deserialSize<T>(m_data) > m_size) return -1; 
		auto sz = ::deserialization(m_data, ref, m_size, flag);
		if (sz == 0) return -1;
		m_data += sz;
		m_size -= sz;
		return 0;
	}
};

#endif