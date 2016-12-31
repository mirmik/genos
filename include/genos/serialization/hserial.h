#ifndef GENOS_HSERIAL
#define GENOS_HSERIAL

#include "genos/serialization.h"

#define SER 1
#define DESER 2
#define SERDIR_MASK 3

template <typename type>
unsigned int hserial(char* buf, type& obj, uint8_t flag) {
	obj.hserial(buf,flag);
};

unsigned int __hserial(char* buf, void* ptr, uint16_t len, uint8_t flag);
unsigned int __hserial_half(char* buf, uint8_t* ref, uint8_t flag);

static inline 
unsigned int hserial_half(char* buf, void* ref, uint8_t flag) {
	return __hserial_half(buf, reinterpret_cast<uint8_t*>(ref), flag);	
};

static inline 
unsigned int hserial(char* buf, int8_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 1, flag);
};

static inline 
unsigned int hserial(char* buf, int16_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 2, flag);
};

static inline 
unsigned int hserial(char* buf, int32_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 4, flag);
};

static inline 
unsigned int hserial(char* buf, int64_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 8, flag);
};

static inline 
unsigned int hserial(char* buf, uint8_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 1, flag);
};

static inline 
unsigned int hserial(char* buf, uint16_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 2, flag);
};

static inline 
unsigned int hserial(char* buf, uint32_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 4, flag);
};

static inline 
unsigned int hserial(char* buf, uint64_t* ref, uint8_t flag) {
	return __hserial(buf, ref, 8, flag);
};

//unsigned int hserial(char* buf, float&, uint8_t flag);
//unsigned int hserial(char* buf, double&, uint8_t flag);

#endif