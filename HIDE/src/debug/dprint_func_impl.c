#include <gxx/debug/dprint.h>
#include <stdint.h>
#include <genos/hal/arch.h>

void debug_printhex_uint4(uint8_t b){
	uint8_t c = b < 10 ? b + '0' : b + 'A' - 10;
	debug_putchar(c);
};

void debug_printhex_uint8 (uint8_t b){
	debug_printhex_uint4((b & 0xF0)>>4);	
	debug_printhex_uint4(b & 0x0F);
};
	
void debug_printbin_uint8 (uint8_t b){
	debug_putchar((b & 0b10000000) ? '1': '0');
	debug_putchar((b & 0b01000000) ? '1': '0');
	debug_putchar((b & 0b00100000) ? '1': '0');
	debug_putchar((b & 0b00010000) ? '1': '0');
	debug_putchar((b & 0b00001000) ? '1': '0');
	debug_putchar((b & 0b00000100) ? '1': '0');
	debug_putchar((b & 0b00000010) ? '1': '0');
	debug_putchar((b & 0b00000001) ? '1': '0');
};

void debug_printhex_uint32(uint32_t a)
{
	uint8_t *p =(uint8_t*) &a;
	debug_printhex_uint8(*(p+3));
	debug_printhex_uint8(*(p+2));
	debug_printhex_uint8(*(p+1));
	debug_printhex_uint8(*(p+0));
};

void debug_printhex_uint64(uint64_t a)
{
	uint8_t *p =(uint8_t*) &a;
	debug_printhex_uint8(*(p+7));
	debug_printhex_uint8(*(p+6));
	debug_printhex_uint8(*(p+5));
	debug_printhex_uint8(*(p+4));
	debug_printhex_uint8(*(p+3));
	debug_printhex_uint8(*(p+2));
	debug_printhex_uint8(*(p+1));
	debug_printhex_uint8(*(p+0));
};

void debug_printhex_uint16(uint16_t a)
{
	uint8_t *p =(uint8_t*) &a;
	debug_printhex_uint8(*(p+1));
	debug_printhex_uint8(*(p+0));
};

void debug_printbin_uint32(uint32_t a)
{
	uint8_t *p =(uint8_t*) &a;
	debug_printbin_uint8(*(p+3));
	debug_printbin_uint8(*(p+2));
	debug_printbin_uint8(*(p+1));
	debug_printbin_uint8(*(p+0));
};

void debug_printbin_uint64(uint64_t a)
{
	uint8_t *p =(uint8_t*) &a;
	debug_printbin_uint8(*(p+7));
	debug_printbin_uint8(*(p+6));
	debug_printbin_uint8(*(p+5));
	debug_printbin_uint8(*(p+4));
	debug_printbin_uint8(*(p+3));
	debug_printbin_uint8(*(p+2));
	debug_printbin_uint8(*(p+1));
	debug_printbin_uint8(*(p+0));
};

void debug_printbin_uint16(uint16_t a)
{
	uint8_t *p =(uint8_t*) &a;
	debug_printbin_uint8(*(p+1));
	debug_printbin_uint8(*(p+0));
};

void debug_printdec_float(float a, int prec)
{
	debug_printdec_double((double)a, prec);
};

void debug_printdec_double(double a, int prec)
{	
	int64_t n = (int64_t) a;
	debug_printdec_int64 ( n );
	debug_putchar('.');
	double o = a - n;
	for(int _iteration = 0; _iteration < prec; ++_iteration ) 
		o *= 10;
	o += 0.5;
	debug_printdec_int64 ( o );
};

#ifdef sp_float
	void debug_printbin_spf(sp_float a)
	{
	uint8_t *p =(uint8_t*) &a;
	debug_printbin_uint8(*(p+3));
	debug_printbin_uint8(*(p+2));
	debug_printbin_uint8(*(p+1));
	debug_printbin_uint8(*(p+0));
	};
#endif

void debug_printhex_ptr(void* v) {
	#if __WORDSIZE == 8 
		debug_printhex_uint8 ((uint8_t )v);
	#elif __WORDSIZE == 16
		debug_printhex_uint16((uint16_t)v);
	#elif __WORDSIZE == 32
		debug_printhex_uint32((uint32_t)v);
	#elif __WORDSIZE == 64
		debug_printhex_uint64((uint64_t)v);
	#else
		#error "Wrong __WORDSIZE"
	#endif
};
	


int debug_strlen(const char *c)
{	
	int i;
	for(i=0;c[i]!=0;i++);
	return i;
};

void debug_print(const char *c)
{	
	debug_write((char*)c, debug_strlen(c));	
};

void debug_print_line()
{	
	debug_putchar('\r');	
	debug_putchar('\n');
};

void debug_print_bool(uint8_t b)
{
	debug_print(b ? "true" : "false");	
};

void debug_print_bool_num(uint8_t b)
{
	debug_putchar(b ? '1' : '0');	
};

void debug_print_test()
{
	debug_putchar('G');
	debug_printhex_uint32(0xABCDEF00);
	debug_putchar('\n');debug_putchar('\r');
	debug_print("DEBUG_print_TEST\n\r");		
};

void debug_print_dump(const void* address, uint16_t size)
{
	//debug_printhex_ptr(address);
	//debug_printhex_uint16(size);
	//while(1);

	uint8_t* dump = (uint8_t*) address; 
	uintptr_t addr_tmp = (uintptr_t) address;
	uint32_t i = 0;
	uint32_t mark = 0;
	
	for (i = 0; i < size; i++)
	{
		
		if (mark == 0)
		{
			debug_printhex_uint32(addr_tmp);
			debug_putchar(':');
			debug_putchar(' ');
		}
		
		debug_printhex_uint8(dump[i]);
		
		if ( mark == 7 )
		{
			debug_putchar('|');
			mark++;
		}
		else if ( mark == 15 )
		{
			debug_putchar('\n');
			debug_putchar('\r');
			mark = 0;
			
			addr_tmp += 0x10;
		}
		else
		{
			debug_putchar(' ');
			mark++;
		}
	} 
	debug_putchar('\n');
	debug_putchar('\r');
}

void debug_print_dump_ascii(const void* address, uint16_t size)
{
	uint8_t* dump = (uint8_t*) address; 
	uintptr_t addr_tmp = (uintptr_t) address;
	uint32_t i = 0;
	uint32_t mark = 0;
	
	for (i = 0; i < size; i++)
	{
		
		if (mark == 0)
		{
			debug_printhex_uint32(addr_tmp);
			debug_putchar(':');
			debug_putchar(' ');
		}
		
		debug_putchar(dump[i]);
		
		if ( mark == 7 )
		{
			debug_putchar(' ');
			mark++;
		}
		else if ( mark == 15 )
		{
			debug_putchar('\n');
			debug_putchar('\r');
			mark = 0;
			
			addr_tmp += 0x10;
		}
		else
		{
			debug_putchar(' ');
			mark++;
		}
	} 
	debug_putchar('\n');
	debug_putchar('\r');
}

void debug_print_dump_simple(void* ptr, uint16_t size)
{
	uint8_t* _ptr = (uint8_t*) ptr;
	while(size--) {debug_printhex_uint8(*_ptr++);};
};

void debug_printdec_int8(int8_t x){	debug_printdec_int64((int64_t)x);};
void debug_printdec_int16(int16_t x){debug_printdec_int64((int64_t)x);};
void debug_printdec_int32(int32_t x){debug_printdec_int64((int64_t)x);};
void debug_printdec_int64(int64_t x){if (x<0) {x=-x; debug_putchar('-');}; debug_printdec_uint64(x);};

void debug_printdec_uint8(uint8_t x){debug_printdec_uint64((uint64_t)x);};
void debug_printdec_uint16(uint16_t x){debug_printdec_uint64((uint64_t)x);};
void debug_printdec_uint32(uint32_t x){debug_printdec_uint64((uint64_t)x);};
void debug_printdec_uint64(uint64_t x){
	char c[24];
	char* end_buf = c + 24;
	char* ptr = end_buf;
	if (x==0) debug_putchar('0');
	*--ptr='\0';
	for (; x != 0; x /= 10)
	{
		*--ptr = ((x % 10) + '0');
	};
	debug_print(ptr);
};

void debug_asmlink_test(){
	debug_putchar('A');
	debug_putchar('B');
	debug_putchar('C');
	debug_putchar('D');
	debug_putchar('E');
	debug_putchar('1');
	debug_putchar('2');
	debug_putchar('3');
	debug_putchar('4');
	debug_putchar('5');
};


void debug_asmlink_uint16_dpr(uint16_t a)
{
	debug_printhex_uint16(a);debug_putchar(':');
};

void debug_asmlink_uint16_dpr2(uint16_t a, uint16_t b)
{
	debug_printhex_uint16(a);debug_putchar(':');
	debug_printhex_uint16(b);debug_putchar(':');
};

void debug_asmlink_uint16_dpr4(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
	debug_printhex_uint16(a);debug_putchar(':');
	debug_printhex_uint16(b);debug_putchar(':');
	debug_printhex_uint16(c);debug_putchar(':');
	debug_printhex_uint16(d);debug_putchar(':');
};





void debug_asmlink_uint8_dpr4(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	debug_printhex_uint8(a);debug_putchar(':');
	debug_printhex_uint8(b);debug_putchar(':');
	debug_printhex_uint8(c);debug_putchar(':');
	debug_printhex_uint8(d);debug_putchar(':');
};

void debug_asmlink_uint8_dpr3(uint8_t a, uint8_t b, uint8_t c)
{
	debug_printhex_uint8(a);debug_putchar(':');
	debug_printhex_uint8(b);debug_putchar(':');
};

void debug_asmlink_uint8_dpr2(uint8_t a, uint8_t b)
{
	debug_printhex_uint8(a);debug_putchar(':');
	debug_printhex_uint8(b);debug_putchar(':');
};

void debug_asmlink_uint8_dpr1(uint8_t a)
{
	debug_printhex_uint8(a);debug_putchar(':');
};




uint16_t debug_asmlink_test_uint16_ret()
{
	return 0xACAB;
};

uint64_t debug_asmlink_test_uint64_ret()
{
	return 0x0123456789ABCDEF;
};
/*	uint16_t debug_simple_test_uint16_2(uint16_t a, uint16_t b)
	{
	debug_printhex_uint16(a);debug_putchar(':');
	debug_printhex_uint16(b);debug_putchar(':');
	return 0xACAB;
	};
	
	uint16_t debug_simple_test_uint16_1(uint16_t a)
	{
	debug_printhex_uint16(a);debug_putchar(':');
	return 0xACAB;
};*/
