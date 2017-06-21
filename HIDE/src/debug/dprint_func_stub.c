#include <debug/dprint.h>

void debug_printhex_uint4(uint8_t b){};

void debug_printhex_uint8 (uint8_t b){};

void debug_printbin_uint4 (uint8_t b){};

void debug_printbin_uint8 (uint8_t b){};

void debug_printhex_uint32(uint32_t a){};

void debug_printhex_uint64(uint64_t a){};

void debug_printhex_uint16(uint16_t a){};

void debug_printbin_uint32(uint32_t a){};

void debug_printbin_uint64(uint64_t a){};

void debug_printbin_uint16(uint16_t a){};

#ifdef sp_float
	void debug_printbin_spf(sp_float a){};
#endif

void debug_printhex_ptr(void* v){};

int debug_strlen(const char *c){};

void debug_print(const char *c){};

void debug_print_bool(uint8_t b){};

void debug_print_bool_num(uint8_t b){};

void debug_print_test(){};

void debug_print_dump(const void* address, uint16_t size){};

void debug_print_dump_ascii(const void* address, uint16_t size){};

void debug_printdec_int8(int8_t x){};
void debug_printdec_int16(int16_t x){};
void debug_printdec_int32(int32_t x){};
void debug_printdec_int64(int64_t x){};

void debug_printdec_uint8(uint8_t x){};
void debug_printdec_uint16(uint16_t x){};
void debug_printdec_uint32(uint32_t x){};
void debug_printdec_uint64(uint64_t x){};

void debug_asmlink_test(){};


void debug_asmlink_uint16_dpr(uint16_t a){};

void debug_asmlink_uint16_dpr2(uint16_t a, uint16_t b){};

void debug_asmlink_uint16_dpr4(uint16_t a, uint16_t b, uint16_t c, uint16_t d){};

uint16_t debug_asmlink_test_uint16_ret()
{
};

uint64_t debug_asmlink_test_uint64_ret()
{
};