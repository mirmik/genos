#ifndef GENOS_SERIALIZATION_H
#define GENOS_SERIALIZATION_H

#include "inttypes.h"
#include "compiler.h"
#include "gxx/buffer.h"
#include "gxx/string.h"
#include "util/bits.h"

#define	SER 1
#define DESER 2
#define SERIAL_SIZE 4
#define SERDIR_MASK 7

#define PROTOORD_LITTLE 4
#define PROTOORD_BIG 8
#define PROTOORD_MASK 12

template <typename type>
unsigned int serialization(char* buf, type& obj, uint8_t flag) {
	obj.serialization(buf,flag);
};

bool is_need_reverse(uint8_t order);

static inline
bool serialization_is_serial(uint8_t flag) {
	return bits_mask(flag, SER);
}

static inline
bool serialization_is_deserial(uint8_t flag) {
	return bits_mask(flag, DESER);
}

unsigned int __rserial(char* buf, void* data, uint16_t len, uint8_t flag);

static inline 
unsigned int rserial(char* buf, char* ref, uint8_t flag) {
	return __rserial(buf, ref, 1, flag);
};

static inline 
unsigned int rserial(char* buf, int8_t* ref, uint8_t flag) {
	return __rserial(buf, ref, 1, flag);
};

static inline 
unsigned int rserial(char* buf, int16_t* ref, uint8_t flag) {
	return __rserial(buf, ref, 2, flag);
};

static inline 
unsigned int rserial(char* buf, int32_t* ref, uint8_t flag) {
	return __rserial(buf, ref, 4, flag);
};

static inline 
unsigned int rserial(char* buf, int64_t* ref, uint8_t flag) {
	return __rserial(buf, ref, 8, flag);
};

template<typename T>
static inline 
unsigned int rserial(char* buf, T* ref, uint8_t flag) {
	return ref->rserial(buf, flag);
};


unsigned int serialization(char* buf, int8_t&, uint8_t flag);
unsigned int serialization(char* buf, int16_t&, uint8_t flag);
unsigned int serialization(char* buf, int32_t&, uint8_t flag);
unsigned int serialization(char* buf, int64_t&, uint8_t flag);
unsigned int serialization(char* buf, uint8_t&, uint8_t flag);
unsigned int serialization(char* buf, uint16_t&, uint8_t flag);
unsigned int serialization(char* buf, uint32_t&, uint8_t flag);
unsigned int serialization(char* buf, uint64_t&, uint8_t flag);

//unsigned int serialization(char* buf, real32_t&, uint8_t flag);

unsigned int serialization(char* buf, gxx::string& ref, uint8_t flag);

unsigned int serialization_raw(char* buf, char* ptr, size_t* len, uint8_t flag);

#endif
