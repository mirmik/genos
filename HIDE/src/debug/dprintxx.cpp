#include "debug/dprint.h"
#include "utilxx/horrible_cast.h"

void dprln() {debug_print("\r\n");};

void dpr(char obj) { debug_putchar(obj); };
void dpr(char* obj) { debug_print(obj); };
void dpr(const char* obj) { debug_print(obj); };

void dpr(uint8_t obj) { debug_printdec_uint8(obj); };
void dpr(uint16_t obj) { debug_printdec_uint16(obj); };
void dpr(uint32_t obj) { debug_printdec_uint32(obj); };
void dpr(uint64_t obj) { debug_printdec_uint64(obj); };

void dpr(int8_t obj) { debug_printdec_int8(obj); };
void dpr(int16_t obj) { debug_printdec_int16(obj); };
void dpr(int32_t obj) { debug_printdec_int32(obj); };
void dpr(int64_t obj) { debug_printdec_int64(obj); };

void dpr(double obj) { debug_printdec_double(obj, 2); };
void dpr(float obj) { debug_printdec_float(obj, 2); };

void dpr(bool obj) { debug_print_bool(obj); };

void dprbin(uint8_t obj) { debug_printbin_uint8(obj); };
void dprbin(uint16_t obj) { debug_printbin_uint16(obj); };
void dprbin(uint32_t obj) { debug_printbin_uint32(obj); };
void dprbin(uint64_t obj) { debug_printbin_uint64(obj); };

void dprbin(int8_t obj) { debug_printbin_uint8(obj); };
void dprbin(int16_t obj) { debug_printbin_uint16(obj); };
void dprbin(int32_t obj) { debug_printbin_uint32(obj); };
void dprbin(int64_t obj) { debug_printbin_uint64(obj); };


void dprhex(const char* obj) { debug_print_dump_simple((void*)obj, debug_strlen(obj));   };
void dprhex(char obj) { debug_printhex_uint8((uint8_t) obj); };

void dprhex(uint8_t obj) { debug_printhex_uint8(obj); };
void dprhex(uint16_t obj) { debug_printhex_uint16(obj); };
void dprhex(uint32_t obj) { debug_printhex_uint32(obj); };
void dprhex(uint64_t obj) { debug_printhex_uint64(obj); };

void dprhex(int8_t obj) { debug_printhex_uint8(obj); };
void dprhex(int16_t obj) { debug_printhex_uint16(obj); };
void dprhex(int32_t obj) { debug_printhex_uint32(obj); };
void dprhex(int64_t obj) { debug_printhex_uint64(obj); };

//template<> void dprhex(float obj) { dprhex(horrible_cast<float_uint, float>(obj)); };
//template<> void dprhex(double obj) { dprhex(horrible_cast<double_uint, double>(obj)); };
//void dpr(gxx::string obj) {dpr(obj.c_str());};

void dpr_dump(void* obj, uint32_t size) 
{ 
	debug_print_dump(obj, size); 
};

void dpr_dump_ascii(void* obj, uint32_t size)
{ 
	debug_print_dump_ascii(obj, size); 
};

void dprtab() {
	debug_putchar('\t');
};

void dprtab(uint8_t obj) {
	while(obj--) {
		debug_putchar('\t');
	}
};