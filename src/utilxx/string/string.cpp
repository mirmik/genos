/*
  WString.cpp - string library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "gxx/string.h"
#include "util/extlibc/itoa.h"
#include "util/ascii_convert.h"
//#include <stdlib.h>
#include "string.h"
#include <stdio.h>
#include "debug/dprint.h"
#include "mem/sysalloc.h"
//#include <ctype.h>

namespace gxx {

/*********************************************/
/*  Constructors                             */
/*********************************************/

string::string(const char *cstr)
{
	init();
	if (cstr) copy(cstr, strlen(cstr));
}

string::string(const string &value)
{
	init();
	*this = value;
}

string::string(string &&rval)
{
	init();
	move(rval);
}
string::string(StringSumHelper &&rval)
{
	init();
	move(rval);
}

string::string(char c)
{
	init();
	char buf[2];
	buf[0] = c;
	buf[1] = 0;
	*this = buf;
}

string::string(unsigned char value, unsigned char base)
{
	init();
	char buf[1 + 8 * sizeof(unsigned char)];
	utoa(value, buf, base);
	*this = buf;
}

string::string(int value, unsigned char base)
{
	init();
	char buf[2 + 8 * sizeof(int)];
	itoa(value, buf, base);
	*this = buf;
}

string::string(unsigned int value, unsigned char base)
{
	init();
	char buf[1 + 8 * sizeof(unsigned int)];
	utoa(value, buf, base);
	*this = buf;
}

string::string(long value, unsigned char base)
{
	init();
	char buf[2 + 8 * sizeof(long)];
	ltoa(value, buf, base);
	*this = buf;
}

string::string(unsigned long value, unsigned char base)
{
	init();
	char buf[1 + 8 * sizeof(unsigned long)];
	ultoa(value, buf, base);
	*this = buf;
}

string::string(const gxx::buffer& cptr)
{
	init();
	if (cptr.data()) copy((const char*)cptr.data(), cptr.size());
}

string::string(const gxx::buffer& cptr, StrBufOpt flag) {
	init();
	const char* src = (const char*) cptr.data();
	
	if (flag == StrBufOpt::DUMP) {
		reserve(cptr.size() * 2 + 1);
		char* dst = (char*) buffer;
		num2hex(dst, cptr.data(), cptr.size());
		len = cptr.size()*2;
		*(buffer + len) = 0;
	}

	if (flag == StrBufOpt::RAW)
		if (cptr.data()) copy((const char*)cptr.data(), cptr.size());			
};


//string::string(float value, unsigned char decimalPlaces)
//{
//	init();
//	char buf[33];
//	*this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
//}

//string::string(double value, unsigned char decimalPlaces)
//{
//	init();
//	char buf[33];
//	*this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
//}

string::~string()
{
	sysfree(buffer);
}

/*********************************************/
/*  Memory Management                        */
/*********************************************/

inline void string::init(void)
{
	buffer = NULL;
	capacity = 0;
	len = 0;
}

void string::invalidate(void)
{
	if (buffer) sysfree(buffer);
	buffer = NULL;
	capacity = len = 0;
}

unsigned char string::reserve(unsigned int size)
{
	if (buffer && capacity >= size) return 1;
	if (changeBuffer(size)) {
		if (len == 0) buffer[0] = 0;
		return 1;
	}
	return 0;
}

unsigned char string::changeBuffer(unsigned int maxStrLen)
{
	char *newbuffer = (char *)sysrealloc(buffer, maxStrLen + 1);
	if (newbuffer) {
		buffer = newbuffer;
		capacity = maxStrLen;
		return 1;
	}
	return 0;
}

/*********************************************/
/*  Copy and Move                            */
/*********************************************/

string & string::copy(const char *cstr, unsigned int length)
{
	if (!reserve(length)) {
		invalidate();
		return *this;
	}
	len = length;
	//strcpy(buffer, cstr);
	memcpy(buffer, cstr, length);
	*(buffer + length) = 0;
	return *this;
}

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

void string::move(string &rhs)
{
	if (buffer) {
		if (capacity >= rhs.len) {
			strcpy(buffer, rhs.buffer);
			len = rhs.len;
			rhs.len = 0;
			return;
		} else {
			sysfree(buffer);
		}
	}
	buffer = rhs.buffer;
	capacity = rhs.capacity;
	len = rhs.len;
	rhs.buffer = NULL;
	rhs.capacity = 0;
	rhs.len = 0;
}

string & string::operator = (const string &rhs)
{
	if (this == &rhs) return *this;
	
	if (rhs.buffer) copy(rhs.buffer, rhs.len);
	else invalidate();
	
	return *this;
}

string & string::operator = (string &&rval)
{
	if (this != &rval) move(rval);
	return *this;
}

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

/*
string & string::operator = (const __FlashStringHelper *pstr)
{
	if (pstr) copy(pstr, strlen_P((PGM_P)pstr));
	else invalidate();

	return *this;
}*/

/*********************************************/
/*  concat                                   */
/*********************************************/

unsigned char string::concat(const string &s)
{
	return concat(s.buffer, s.len);
}

unsigned char string::concat(const char *cstr, unsigned int length)
{
	unsigned int newlen = len + length;
	if (!cstr) return 0;
	if (length == 0) return 1;
	if (!reserve(newlen)) return 0;
	//strcpy(buffer + len, cstr);
	memcpy(buffer + len, cstr, length);
	*(buffer + newlen) = 0;
	len = newlen;
	return 1;
}

unsigned char string::concat(const char *cstr)
{
	if (!cstr) return 0;
	return concat(cstr, strlen(cstr));
}

unsigned char string::concat(char c)
{
	char buf[2];
	buf[0] = c;
	buf[1] = 0;
	return concat(buf, 1);
}

unsigned char string::concat(unsigned char num)
{
	char buf[1 + 3 * sizeof(unsigned char)];
	itoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

unsigned char string::concat(int num)
{
	char buf[2 + 3 * sizeof(int)];
	itoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

unsigned char string::concat(unsigned int num)
{
	char buf[1 + 3 * sizeof(unsigned int)];
	utoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

unsigned char string::concat(long num)
{
	char buf[2 + 3 * sizeof(long)];
	ltoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

unsigned char string::concat(unsigned long num)
{
	char buf[1 + 3 * sizeof(unsigned long)];
	ultoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

unsigned char string::concat(const gxx::buffer& cptr) {
	return concat((const char*)cptr.data(), cptr.size());
}

//unsigned char string::concat(float num)
//{
//	char buf[20];
//	char* string = dtostrf(num, 4, 2, buf);
//	return concat(string, strlen(string));
//}

//unsigned char string::concat(double num)
//{
//	char buf[20];
//	char* string = dtostrf(num, 4, 2, buf);
//	return concat(string, strlen(string));
//}



/*
unsigned char string::concat(const __FlashStringHelper * str)
{
	if (!str) return 0;
	int length = strlen_P((const char *) str);
	if (length == 0) return 1;
	unsigned int newlen = len + length;
	if (!reserve(newlen)) return 0;
	strcpy_P(buffer + len, (const char *) str);
	len = newlen;
	return 1;
}*/

/*********************************************/
/*  Concatenate                              */
/*********************************************/

StringSumHelper & operator + (const StringSumHelper &lhs, const string &rhs)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(rhs.buffer, rhs.len)) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, const char *cstr)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!cstr || !a.concat(cstr, strlen(cstr))) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, char c)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(c)) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, unsigned char num)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(num)) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, int num)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(num)) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, unsigned int num)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(num)) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, long num)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(num)) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, unsigned long num)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(num)) a.invalidate();
	return a;
}

/*
StringSumHelper & operator + (const StringSumHelper &lhs, float num)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(num)) a.invalidate();
	return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, double num)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(num)) a.invalidate();
	return a;
}*/
/*
StringSumHelper & operator + (const StringSumHelper &lhs, const __FlashStringHelper *rhs)
{
	StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
	if (!a.concat(rhs))	a.invalidate();
	return a;
}*/

/*********************************************/
/*  Comparison                               */
/*********************************************/

int string::compareTo(const string &s) const
{
	if (!buffer || !s.buffer) {
		if (s.buffer && s.len > 0) return 0 - *(unsigned char *)s.buffer;
		if (buffer && len > 0) return *(unsigned char *)buffer;
		return 0;
	}
	return strcmp(buffer, s.buffer);
}

unsigned char string::equals(const string &s2) const
{
	return (len == s2.len && compareTo(s2) == 0);
}

unsigned char string::equals(const char *cstr) const
{
	if (len == 0) return (cstr == NULL || *cstr == 0);
	if (cstr == NULL) return buffer[0] == 0;
	return strcmp(buffer, cstr) == 0;
}

unsigned char string::operator<(const string &rhs) const
{
	return compareTo(rhs) < 0;
}

unsigned char string::operator>(const string &rhs) const
{
	return compareTo(rhs) > 0;
}

unsigned char string::operator<=(const string &rhs) const
{
	return compareTo(rhs) <= 0;
}

unsigned char string::operator>=(const string &rhs) const
{
	return compareTo(rhs) >= 0;
}
/*
unsigned char string::equalsIgnoreCase( const string &s2 ) const
{
	if (this == &s2) return 1;
	if (len != s2.len) return 0;
	if (len == 0) return 1;
	const char *p1 = buffer;
	const char *p2 = s2.buffer;
	while (*p1) {
		if (tolower(*p1++) != tolower(*p2++)) return 0;
	} 
	return 1;
}*/

unsigned char string::startsWith( const string &s2 ) const
{
	if (len < s2.len) return 0;
	return startsWith(s2, 0);
}

unsigned char string::startsWith( const string &s2, unsigned int offset ) const
{
	if (offset > len - s2.len || !buffer || !s2.buffer) return 0;
	return strncmp( &buffer[offset], s2.buffer, s2.len ) == 0;
}

unsigned char string::endsWith( const string &s2 ) const
{
	if ( len < s2.len || !buffer || !s2.buffer) return 0;
	return strcmp(&buffer[len - s2.len], s2.buffer) == 0;
}

/*********************************************/
/*  Character Access                         */
/*********************************************/

char string::charAt(unsigned int loc) const
{
	return operator[](loc);
}

void string::setCharAt(unsigned int loc, char c) 
{
	if (loc < len) buffer[loc] = c;
}

char & string::operator[](unsigned int index)
{
	static char dummy_writable_char;
	if (index >= len || !buffer) {
		dummy_writable_char = 0;
		return dummy_writable_char;
	}
	return buffer[index];
}

char string::operator[]( unsigned int index ) const
{
	if (index >= len || !buffer) return 0;
	return buffer[index];
}

void string::getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index) const
{
	if (!bufsize || !buf) return;
	if (index >= len) {
		buf[0] = 0;
		return;
	}
	unsigned int n = bufsize - 1;
	if (n > len - index) n = len - index;
	strncpy((char *)buf, buffer + index, n);
	buf[n] = 0;
}

/*********************************************/
/*  Search                                   */
/*********************************************/

int string::indexOf(char c) const
{
	return indexOf(c, 0);
}

int string::indexOf( char ch, unsigned int fromIndex ) const
{
	if (fromIndex >= len) return -1;
	const char* temp = strchr(buffer + fromIndex, ch);
	if (temp == NULL) return -1;
	return temp - buffer;
}

int string::indexOf(const string &s2) const
{
	return indexOf(s2, 0);
}

int string::indexOf(const string &s2, unsigned int fromIndex) const
{
	if (fromIndex >= len) return -1;
	const char *found = strstr(buffer + fromIndex, s2.buffer);
	if (found == NULL) return -1;
	return found - buffer;
}

int string::lastIndexOf( char theChar ) const
{
	return lastIndexOf(theChar, len - 1);
}

int string::lastIndexOf(char ch, unsigned int fromIndex) const
{
	if (fromIndex >= len) return -1;
	char tempchar = buffer[fromIndex + 1];
	buffer[fromIndex + 1] = '\0';
	char* temp = strrchr( buffer, ch );
	buffer[fromIndex + 1] = tempchar;
	if (temp == NULL) return -1;
	return temp - buffer;
}

int string::lastIndexOf(const string &s2) const
{
	return lastIndexOf(s2, len - s2.len);
}

int string::lastIndexOf(const string &s2, unsigned int fromIndex) const
{
  	if (s2.len == 0 || len == 0 || s2.len > len) return -1;
	if (fromIndex >= len) fromIndex = len - 1;
	int found = -1;
	for (char *p = buffer; p <= buffer + fromIndex; p++) {
		p = strstr(p, s2.buffer);
		if (!p) break;
		if ((unsigned int)(p - buffer) <= fromIndex) found = p - buffer;
	}
	return found;
}

string string::substring(unsigned int left, unsigned int right) const
{
	if (left > right) {
		unsigned int temp = right;
		right = left;
		left = temp;
	}
	string out;
	if (left >= len) return out;
	if (right > len) right = len;
	char temp = buffer[right];  // save the replaced character
	buffer[right] = '\0';	
	out = buffer + left;  // pointer arithmetic
	buffer[right] = temp;  //restore character
	return out;
}

/*********************************************/
/*  Modification                             */
/*********************************************/

void string::replace(char find, char replace)
{
	if (!buffer) return;
	for (char *p = buffer; *p; p++) {
		if (*p == find) *p = replace;
	}
}

void string::replace(const string& find, const string& replace)
{
	if (len == 0 || find.len == 0) return;
	int diff = replace.len - find.len;
	char *readFrom = buffer;
	char *foundAt;
	if (diff == 0) {
		while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
			memcpy(foundAt, replace.buffer, replace.len);
			readFrom = foundAt + replace.len;
		}
	} else if (diff < 0) {
		char *writeTo = buffer;
		while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
			unsigned int n = foundAt - readFrom;
			memcpy(writeTo, readFrom, n);
			writeTo += n;
			memcpy(writeTo, replace.buffer, replace.len);
			writeTo += replace.len;
			readFrom = foundAt + find.len;
			len += diff;
		}
		strcpy(writeTo, readFrom);
	} else {
		unsigned int size = len; // compute size needed for result
		while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
			readFrom = foundAt + find.len;
			size += diff;
		}
		if (size == len) return;
		if (size > capacity && !changeBuffer(size)) return; // XXX: tell user!
		int index = len - 1;
		while (index >= 0 && (index = lastIndexOf(find, index)) >= 0) {
			readFrom = buffer + index + find.len;
			memmove(readFrom + diff, readFrom, len - (readFrom - buffer));
			len += diff;
			buffer[len] = 0;
			memcpy(buffer + index, replace.buffer, replace.len);
			index--;
		}
	}
}

void string::remove(unsigned int index){
	// Pass the biggest integer as the count. The remove method
	// below will take care of truncating it at the end of the
	// string.
	remove(index, (unsigned int)-1);
}

void string::remove(unsigned int index, unsigned int count){
	if (index >= len) { return; }
	if (count <= 0) { return; }
	if (count > len - index) { count = len - index; }
	char *writeTo = buffer + index;
	len = len - count;
	strncpy(writeTo, buffer + index + count,len - index);
	buffer[len] = 0;
}
/*
void string::toLowerCase(void)
{
	if (!buffer) return;
	for (char *p = buffer; *p; p++) {
		*p = tolower(*p);
	}
}

void string::toUpperCase(void)
{
	if (!buffer) return;
	for (char *p = buffer; *p; p++) {
		*p = toupper(*p);
	}
}
*/
/*
void string::trim(void)
{
	if (!buffer || len == 0) return;
	char *begin = buffer;
	while (isspace(*begin)) begin++;
	char *end = buffer + len - 1;
	while (isspace(*end) && end >= begin) end--;
	len = end + 1 - begin;
	if (begin > buffer) memcpy(buffer, begin, len);
	buffer[len] = 0;
}
*/
/*********************************************/
/*  Parsing / Conversion                     */
/*********************************************/

long string::toInt(void) const
{
	if (buffer) return atol(buffer);
	return 0;
}

float string::toFloat(void) const
{
	if (buffer) return float(atof(buffer));
	return 0;
}

};

