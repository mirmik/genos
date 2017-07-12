#ifndef UTIL_NUMBER_CONVERT
#define UTIL_NUMBER_CONVERT

#include <genos/defines/size_t.h>

static inline char *i64toa( int64_t num, char *buf, uint8_t base )
{
	char *p = buf;
	char *p1, *p2;
	uint64_t ud = 0;

	*buf = '\0';	/* initialize buffer. In the case of an error, this will already be in the buffer, indicating that the result is invalid (NULL). */
	p1 = buf;	/* start of buffer */

	// check base
	if( base < 2 || base > 36 )
		{ return buf; }

	/* If num < 0, put `-' in the head.  */
	if( num < 0 )
	{
		*(p++) = '-';
		p1++;
		ud = -num;
	}
	else
		{ ud = num; }

	/* Divide ud by base until ud == 0.  */
	int16_t remainder = 0;
	do {
		remainder = ud % base;
		*(p++) = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while( ud /= base );

	/* Terminate buf.  */
	*p = '\0';

	/* Reverse buffer.  */
	p2 = p - 1;	/* end of buffer */
	char tmp;
	while( p1 < p2 )
	{
		tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	return buf;
}

static inline char *i32toa( int32_t num, char *buf, uint8_t base ) {
	return i64toa(num, buf, base);
}

static inline char *i16toa( int16_t num, char *buf, uint8_t base ) {
	return i64toa(num, buf, base);
}

static inline char *i8toa( int8_t num, char *buf, uint8_t base ) {
	return i64toa(num, buf, base);
}

static inline char *u64toa( uint64_t num, char *buf, uint8_t base )
{
	char *p = buf;
	char *p1, *p2;
	uint64_t ud = 0;

	*buf = '\0';	/* initialize buffer. In the case of an error, this will already be in the buffer, indicating that the result is invalid (NULL). */
	p1 = buf;	/* start of buffer */

	// check base
	if( base < 2 || base > 36 )
		{ return buf; }

	ud = num;

	/* Divide ud by base until ud == 0.  */
	int16_t remainder = 0;
	do {
		remainder = ud % base;
		*(p++) = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while( ud /= base );

	/* Terminate buf.  */
	*p = '\0';

	/* Reverse buffer.  */
	p2 = p - 1;	/* end of buffer */
	char tmp;
	while( p1 < p2 )
	{
		tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	return buf;
}

static inline char *u32toa( uint64_t num, char *buf, uint8_t base ) {
	return u64toa(num, buf, base);
}

static inline char *u16toa( uint64_t num, char *buf, uint8_t base ) {
	return u64toa(num, buf, base);
}

static inline char *u8toa( uint64_t num, char *buf, uint8_t base ) {
	return u64toa(num, buf, base);
}

#endif