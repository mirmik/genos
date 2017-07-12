#ifndef ASCII_CONVERT_H
#define ASCII_CONVERT_H

#include "inttypes.h"
#include "util/accessor.h"
#include "gxx/charbuf.h"

using namespace gxx;

static inline uint8_t hex2hbyte(char c) 
{
	return (uint8_t)(c <= '9' ? c - '0' : c - 'A' + 10);
};

static inline char hbyte2hex(uint8_t n) 
{
	return n < 10 ? '0' + n : 'A' - 10 + n;
};

static inline uint8_t dec2hbyte(char c) 
{
	return (uint8_t)(c - '0');
};

static inline char hbyte2dec(uint8_t n) 
{
	return '0' + n;
};

static inline uint8_t hex2byte(const char* str) 
{
	uint8_accessor ret;
	ret.h = hex2hbyte(*str++);
	ret.l = hex2hbyte(*str);
	return ret.num;
};

static inline void byte2hex(char* buf, uint8_t src)
{
	uint8_accessor acs; acs.num = src;
	*buf++ = hbyte2hex(acs.h); 
	*buf++ = hbyte2hex(acs.l); 
};

static inline void hex2bytes(void* num, const char* hex, int numlen) {
	char* start = (char*) num;
	char* stop = (char*) num + numlen;
	for (char* ptr = start; ptr != stop; ++ptr) {
		*ptr = hex2byte(hex); 
		hex += 2;
	};
};

static inline void hex2bytes_swap(void* num, const char* hex, int numlen) {
	char* start = (char*) num + numlen - 1;
	char* stop = (char*) num - 1;
	for (char* ptr = start; ptr != stop; --ptr) {
		*ptr = hex2byte(hex);  
		hex += 2;
	};
};

static inline void bytes2hex(char* hex, const void* num, int numlen) {
	char* start = (char*) num;
	char* stop = (char*) num + numlen;
	for (char* ptr = start; ptr != stop; ++ptr) {
		byte2hex(hex, *ptr);
		hex += 2;
	};
};

static inline void bytes2hex_swap(char* hex, const void* num, int numlen) {
	char* start = (char*) num + numlen - 1;
	char* stop = (char*) num - 1;
	for (char* ptr = start; ptr != stop; --ptr) {
		byte2hex(hex, *ptr);
		hex += 2;
	};
};
//#define get_halfbyte_number(u,n) (u>>(4*(n)) & 0xF)

static inline void uint8_to_hex(char* buf, uint8_t src)
{
	uint8_accessor acs; acs.num = src;
	*buf++ = hbyte2hex(acs.h); 
	*buf++ = hbyte2hex(acs.l); 
};

static inline void uint16_to_hex(char* buf, uint16_t src)
{
	uint16_accessor acs; acs.num = src;
	*buf++ = hbyte2hex(acs.hi_h); 
	*buf++ = hbyte2hex(acs.hi_l); 
	*buf++ = hbyte2hex(acs.lo_h); 
	*buf++ = hbyte2hex(acs.lo_l); 
};

static inline void uint32_to_hex(char* buf, uint32_t src)
{
	uint32_accessor acs; acs.num = src;
	*buf++ = hbyte2hex(acs.hhi_h); 
	*buf++ = hbyte2hex(acs.hhi_l); 
	*buf++ = hbyte2hex(acs.hlo_h); 
	*buf++ = hbyte2hex(acs.hlo_l);
	*buf++ = hbyte2hex(acs.lhi_h); 
	*buf++ = hbyte2hex(acs.lhi_l); 
	*buf++ = hbyte2hex(acs.llo_h); 
	*buf++ = hbyte2hex(acs.llo_l);
};

static inline void uint64_to_hex(char* buf, uint64_t src)
{
	uint64_accessor acs; acs.num = src;
	*buf++ = hbyte2hex(acs.hhhi_h); 
	*buf++ = hbyte2hex(acs.hhhi_l); 
	*buf++ = hbyte2hex(acs.hhlo_h); 
	*buf++ = hbyte2hex(acs.hhlo_l);
	*buf++ = hbyte2hex(acs.hlhi_h); 
	*buf++ = hbyte2hex(acs.hlhi_l); 
	*buf++ = hbyte2hex(acs.hllo_h); 
	*buf++ = hbyte2hex(acs.hllo_l);
	*buf++ = hbyte2hex(acs.lhhi_h); 
	*buf++ = hbyte2hex(acs.lhhi_l); 
	*buf++ = hbyte2hex(acs.lhlo_h); 
	*buf++ = hbyte2hex(acs.lhlo_l);
	*buf++ = hbyte2hex(acs.llhi_h); 
	*buf++ = hbyte2hex(acs.llhi_l); 
	*buf++ = hbyte2hex(acs.lllo_h); 
	*buf++ = hbyte2hex(acs.lllo_l);
};

static inline uint8_t hex_to_uint4(const char* str)
{
	return hex2hbyte(*str);
};

static inline uint8_t hex_to_uint8(const char* str)
{
	return hex2byte(str);
};

static inline uint16_t hex_to_uint16(const char* str)
{
	uint16_accessor s;
	s.hi = hex2byte(str+0);
	s.lo = hex2byte(str+2);
	return s.num;
};

static inline uint32_t hex_to_uint32(const char* str)
{
	uint32_accessor s;
	s.hhi = hex2byte(str+0);
	s.hlo = hex2byte(str+2);
	s.lhi = hex2byte(str+4);
	s.llo = hex2byte(str+6);
	return s.num;
};


static inline uint64_t hex_to_uint64(const char* str)
{
	uint64_accessor s;
	s.hhhi = hex2byte(str+0);
	s.hhlo = hex2byte(str+2);
	s.hlhi = hex2byte(str+4);
	s.hllo = hex2byte(str+6);
	s.lhhi = hex2byte(str+8);
	s.lhlo = hex2byte(str+10);
	s.llhi = hex2byte(str+12);
	s.lllo = hex2byte(str+14);
	return s.num;
};

static inline int8_t hex_to_int8(const char* ptr)
{ return static_cast<int8_t>(hex_to_uint8(ptr)); };

static inline int16_t hex_to_int16(const char* ptr)
{ return static_cast<int16_t>(hex_to_uint16(ptr)); };

static inline int32_t hex_to_int32(const char* ptr)
{ return static_cast<int32_t>(hex_to_uint32(ptr)); }

static inline int64_t hex_to_int64(const char* ptr)
{ return static_cast<int64_t>(hex_to_uint64(ptr)); }


#if BYTE_ORDER == __LITTLE_ENDIAN
static inline void bytes2lhex(char* hexdst, const void* bytessrc, size_t bytesTotal) { return bytes2hex 		(hexdst, bytessrc, bytesTotal); }
static inline void bytes2bhex(char* hexdst, const void* bytessrc, size_t bytesTotal) { return bytes2hex_swap 	(hexdst, bytessrc, bytesTotal); }

static inline void lhex2bytes(void* bytesdst, const char* hexsrc, size_t bytesTotal) { return hex2bytes 		(bytesdst, hexsrc, bytesTotal); }
static inline void bhex2bytes(void* bytesdst, const char* hexsrc, size_t bytesTotal) { return hex2bytes_swap	(bytesdst, hexsrc, bytesTotal); }

#elif BYTE_ORDER == __BIG_ENDIAN
static inline void bytes2lhex(char* hexdst, const void* bytessrc, size_t bytesTotal) { return bytes2hex_swap 	(hexdst, bytessrc, bytesTotal); }
static inline void bytes2bhex(char* hexdst, const void* bytessrc, size_t bytesTotal) { return bytes2hex 		(hexdst, bytessrc, bytesTotal); }

static inline void lhex2bytes(void* bytesdst, const char* hexsrc, size_t bytesTotal) { return hex2bytes_swap	(bytesdst, hexsrc, bytesTotal); }
static inline void bhex2bytes(void* bytesdst, const char* hexsrc, size_t bytesTotal) { return hex2bytes			(bytesdst, hexsrc, bytesTotal); }

#else
	#error "WrongByteOrder"
#endif
#endif