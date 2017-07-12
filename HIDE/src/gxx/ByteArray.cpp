#include <gxx/ByteArray.h>
#include <gxx/debug/dprint.h>
#include "util/numconvert.h"
//#include "util/ascii_convert.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include <gxx/algorithm.h>

#ifndef DEBUG
#define DEBUG 0
#endif

namespace gxx {

	ByteArray::ByteArray()  : m_data(nullptr), m_capacity(0), m_size(0), BasicAllocated() {

	}

	ByteArray::ByteArray(const ByteArray& other) : ByteArray() {
		if (DEBUG) printf("str(const str&)\n");
		*this = other;
	}

	ByteArray::ByteArray(ByteArray&& other) : ByteArray() {
		if (DEBUG) printf("str(str&&)\n");
		move(other);
	}

	ByteArray::ByteArray(const char* str) : ByteArray() {
		if (DEBUG) printf("str(const char*)\n");
		if (str) copy(str, strlen(str));
	}

	//ByteArray::ByteArray(const gxx::buffer& buf) : ByteArray() {
	//	if (DEBUG) printf("str(const str&)\n");
	//	if (buf.data()) copy((const char*)buf.data(), buf.size());
	//};

	ByteArray::~ByteArray() {
		if (DEBUG) printf("~str\n");
		if (m_data) m_alloc->deallocate(m_data);
	};


	ByteArray & ByteArray::copy(const char *cstr, size_t length) {
		if (!reserve(length)) {
			invalidate();
			return *this;
		}
		m_size = length;
		memcpy(m_data, cstr, length);
		return *this;
	}


	void ByteArray::move(ByteArray &rhs) {
		if (DEBUG) printf("deallocate %s\n", rhs.c_str());
		if (DEBUG) printf("deallocate %p\n", m_data);
		if (m_data) m_alloc->deallocate(m_data);
		if (DEBUG) printf("end deallocate\n");
		m_data = rhs.m_data;
		m_capacity = rhs.m_capacity;
		m_size = rhs.m_size;
		m_alloc = rhs.m_alloc;
		rhs.m_data = nullptr;
		rhs.m_capacity = 0;
		rhs.m_size = 0;
		if (DEBUG) printf("end move\n");
	}


	void ByteArray::invalidate(void) {
		if (m_data) m_alloc->deallocate(m_data);
		m_data = nullptr;
		m_capacity = m_size = 0;
	}

	ByteArray & ByteArray::operator = (const ByteArray &rhs) {
		if (DEBUG) printf("=str(const str&) ");
		if (this == &rhs) return *this;
		
		if (rhs.m_data) copy(rhs.m_data, rhs.m_size);
		else invalidate();
		
		if (DEBUG) printf("%s\n", c_str());
		return *this;
	}
	
	ByteArray & ByteArray::operator = (ByteArray &&rval) {
		if (DEBUG) printf("=str(str&&)\n");
		if (this != &rval) move(rval);
		return *this;
	}

	const char* ByteArray::c_str() {
		reserve(m_size + 1);
		*end() = 0;
		return begin();
	};

	char* ByteArray::begin() {
		return m_data;
	};

	char* ByteArray::end() {
		return m_data + m_size;		
	};


	unsigned char ByteArray::reserve(size_t sz) {
		if (m_data && m_capacity >= sz) return 1;
		if (changeBuffer(sz)) {
			return 1;
		}
		return 0;
	}

	unsigned char ByteArray::changeBuffer(size_t maxStrLen) {
		char *newbuf = (char *)m_alloc->reallocate(m_data, maxStrLen);
		if (newbuf) {
			m_data = newbuf;
			m_capacity = maxStrLen;
			return 1;
		}
		return 0;
	}

	unsigned char ByteArray::concat(const char *cstr, size_t length) {
		size_t newlen = m_size + length;
		if (!cstr) return 0;
		if (length == 0) return 1;
		if (!reserve(newlen)) return 0;
		memcpy(m_data + m_size, cstr, length);
		m_size = newlen;
		return 1;
	}
	
	unsigned char ByteArray::concat(char c) {
		return concat(&c, 1);
	}

	unsigned char ByteArray::concat(const char *cstr) {
		if (!cstr) return 0;
		return concat(cstr, strlen(cstr));
	}

	unsigned char ByteArray::concat(int8_t num, uint8_t base) {
		char buf[4];
		i8toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(int16_t num, uint8_t base) {
		char buf[7];
		i16toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(int32_t num, uint8_t base) {
		char buf[12];
		i32toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(int64_t num, uint8_t base) {
		char buf[22];
		i64toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(uint8_t num, uint8_t base) {
		char buf[4];
		u8toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(uint16_t num, uint8_t base) {
		char buf[7];
		u16toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(uint32_t num, uint8_t base) {
		char buf[12];
		u32toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(uint64_t num, uint8_t base) {
		char buf[22];
		u64toa(num, buf, base);
		return concat(buf);
	}

	unsigned char ByteArray::concat(const ByteArray& other) {
		return concat(other.data(), other.size());
	}

	ByteArray ByteArray::number(uint8_t num, uint8_t base) {
		char buf[4];
		u8toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};

	ByteArray ByteArray::number(uint16_t num, uint8_t base) {
		char buf[7];
		u16toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};

	ByteArray ByteArray::number(uint32_t num, uint8_t base) {
		char buf[12];
		u32toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};
	
	ByteArray ByteArray::number(uint64_t num, uint8_t base) {
		char buf[22];
		u64toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};

	ByteArray ByteArray::number(int8_t num, uint8_t base) {
		char buf[4];
		i8toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};

	ByteArray ByteArray::number(int16_t num, uint8_t base) {
		char buf[7];
		i16toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};

	ByteArray ByteArray::number(int32_t num, uint8_t base) {
		char buf[12];
		i32toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};
	
	ByteArray ByteArray::number(int64_t num, uint8_t base) {
		char buf[22];
		i64toa(num, buf, base);
		return gxx::move(ByteArray(buf));
	};

	ByteArray ByteArray::number(double num, char format, uint8_t prec) {
		char buf[64];
		char fmt[] = "%.*x";
		fmt[3] = format;
		sprintf(buf, fmt, prec, num); 
		return ByteArray(buf);
	};


	bool ByteArray::operator < (const ByteArray& other) const {
		return strncmp(data(), other.data(), gxx::min(size(), other.size())) < 0;
	};

	//ByteArray hexdata(const void* data, size_t sz) {
	//	ByteArray buf;
	//	buf.reserve(sz * 2 + 1);
	//	char* dst = buf.data();
	//	const uint8_t* src = (const uint8_t*) data;
	//	for (int i = 0; i < sz; i++) {
	//		byte2hex(dst, *src++);
	//		dst = dst + 2;
	//	}
	//};

	ByteArray ByteArray::format(const char* fmt, ...) {
		char buf[128];

		va_list args;
		//assert(format != NULL);
		va_start(args, format);
		vsprintf(buf, fmt, args); 
		va_end(args);

		return gxx::move(ByteArray(buf));
	};

	/*static ByteArray number(uint16_t num, uint8_t base = 10);
	static ByteArray number(uint32_t num, uint8_t base = 10);
	static ByteArray number(uint64_t num, uint8_t base = 10);
		
	static ByteArray number(int8_t num, uint8_t base = 10);
	static ByteArray number(int16_t num, uint8_t base = 10);
	static ByteArray number(int32_t num, uint8_t base = 10);
	static ByteArray number(int64_t num, uint8_t base = 10);
*/

	ByteArray & ByteArray::shrink() {
		changeBuffer(m_size);
		return *this;
	}

	ByteArray & ByteArray::shrink_to_print() {
		changeBuffer(m_size + 1);
		return *this;
	}
	
	/*size_t ByteArray::size() const {
		return m_size;
	}; 

	size_t ByteArray::capacity() const {
		return m_capacity;
	};

	char* ByteArray::data() const {
		return m_data;
	};*/

/*string & string::copy(const __FlashStringHelper *pstr, unsigned int length)
{
	if (!reserve(length)) {
		invalidate();
		return *this;
	}
	len = length;
	strcpy_P(buffer, (PGM_P)pstr);
	return *this;
}*/
/*

string & string::operator = (StringSumHelper &&rval)
{
	if (this != &rval) move(rval);
	return *this;
}

string & string::operator = (const char *cstr)
{
	if (cstr) copy(cstr, strlen(cstr));
	else invalidate();
	
	return *this;
}

string & string::operator = (const gxx::buffer& cptr)
{
	if (cptr.data()) copy((const char*)cptr.data(), cptr.size());
	else invalidate();
	
	return *this;
}

*/



/*
	ByteArray::~ByteArray() {
		release();
	}	

	void ByteArray::release() {
		m_record->refs--;
		if (m_record->refs <= 0) {
			if (m_record->data) m_alloc->deallocate(m_record->data);
			m_alloc->destructObject(m_record);
		}
	}

	uint8_t ByteArray::reserve(size_t sz) {
		if (capacity() >= sz) return 1;
		return changeBuffer(sz);
	}

	uint8_t ByteArray::changeBuffer(size_t sz) {
		char* oldbuf = m_record->data;
		char* newbuf = (char*)m_alloc->allocate(sz);
		if (!oldbuf) memcpy(newbuf, oldbuf, m_record->size);
		m_record->capacity = sz;
		m_record->data = newbuf;	
		if (oldbuf) m_alloc->deallocate(oldbuf);
		return 1;
	}

	size_t ByteArray::size() const {
		return m_record->size;
	};

	size_t ByteArray::capacity() const {
		return m_record->capacity;
	};


	char* ByteArray::data() {
		return m_record->data;
	};

	ByteArray& ByteArray::operator= (const ByteArray& other) {
		release();
		m_record = other.m_record;
		m_record->refs++;		
	}
*/
};