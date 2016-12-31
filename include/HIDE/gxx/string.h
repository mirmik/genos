/*
  WString.h - string library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
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

#ifndef String_class_h
#define String_class_h

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gxx/buffer.h"
#include "gxx/charbuf.h"

//#include "glib/utilxx/to_string.h"

namespace gxx {

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

//class __FlashStringHelper;
//#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class StringSumHelper;

enum StrBufOpt {
	RAW,
	DUMP
};

// The string class
class string
{
	// use a function pointer to allow for "if (s)" without the
	// complications of an operator bool(). for more information, see:
	// http://www.artima.com/cppsource/safebool.html
	typedef void (string::*StringIfHelperType)() const;
	void StringIfHelper() const {}

public:
	// constructors
	// creates a copy of the initial value.
	// if the initial value is null or invalid, or if memory allocation
	// fails, the string will be marked as invalid (i.e. "if (s)" will
	// be false).
	string(const char *cstr = "");
	string(const string &str);
	string(string &&rval);
	string(StringSumHelper &&rval);
	
	explicit string(const gxx::buffer& cptr);
	explicit string(const gxx::buffer& cptr, StrBufOpt flag);
    
	explicit string(char c);
	explicit string(unsigned char, unsigned char base=10);
	explicit string(int, unsigned char base=10);
	explicit string(unsigned int, unsigned char base=10);
	explicit string(long, unsigned char base=10);
	explicit string(unsigned long, unsigned char base=10);
	explicit string(float, unsigned char decimalPlaces=2);
	explicit string(double, unsigned char decimalPlaces=2);
	
	~string(void);

	// memory management
	// return true on success, false on failure (in which case, the string
	// is left unchanged).  reserve(0), if successful, will validate an
	// invalid string (i.e., "if (s)" will be true afterwards)
	unsigned char reserve(unsigned int size);
	inline unsigned int length(void) const {return len;}

	// creates a copy of the assigned value.  if the value is null or
	// invalid, or if the memory allocation fails, the string will be 
	// marked as invalid ("if (s)" will be false).
	string & operator = (const string &rhs);
	string & operator = (const char *cstr);
	//string & operator = (const __FlashStringHelper *str);

	string & operator = (const gxx::buffer& cptr);

    #if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
	string & operator = (string &&rval);
	string & operator = (StringSumHelper &&rval);
	#endif

	// concatenate (works w/ built-in types)
	
	// returns true on success, false on failure (in which case, the string
	// is left unchanged).  if the argument is null or invalid, the 
	// concatenation is considered unsucessful.  
	unsigned char concat(const string &str);
	unsigned char concat(const char *cstr);
	unsigned char concat(char c);
	unsigned char concat(unsigned char c);
	unsigned char concat(int num);
	unsigned char concat(unsigned int num);
	unsigned char concat(long num);
	unsigned char concat(unsigned long num);
	unsigned char concat(float num);
	unsigned char concat(double num);
	unsigned char concat(const gxx::buffer& cptr);
	//unsigned char concat(gxx::buffer&& cptr);
	//unsigned char concat(const __FlashStringHelper * str);
	
	template<unsigned int size>
	unsigned char concat(charbuf<size> buf)
	{
		return concat(buf.to_buf(), buf.length());
	};

	// if there's not enough memory for the concatenated value, the string
	// will be left unchanged (but this isn't signalled in any way)
	string & operator << (const string &rhs){concat(rhs); return (*this);}
	string & operator << (const char *cstr)	{concat(cstr); return (*this);}
	string & operator << (char c)			{concat(c); return (*this);}
	string & operator << (unsigned char num){concat(num); return (*this);}
	string & operator << (int num)			{concat(num); return (*this);}
	string & operator << (unsigned int num)	{concat(num); return (*this);}
	string & operator << (long num)			{concat(num); return (*this);}
	string & operator << (unsigned long num){concat(num); return (*this);}
	string & operator << (float num)		{concat(num); return (*this);}
	string & operator << (double num)		{concat(num); return (*this);}
	string & operator << (const gxx::buffer& cptr) {concat(cptr); return (*this);}

	template<unsigned int size>
	string & operator << (charbuf<size> buf){concat(buf); return (*this);}
	//string & operator += (const __FlashStringHelper *str){concat(str); return (*this);}

	friend StringSumHelper & operator + (const StringSumHelper &lhs, const string &rhs);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, const char *cstr);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, char c);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, unsigned char num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, int num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, unsigned int num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, long num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, unsigned long num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, float num);
	friend StringSumHelper & operator + (const StringSumHelper &lhs, double num);
	//friend StringSumHelper & operator + (const StringSumHelper &lhs, const __FlashStringHelper *rhs);

	// comparison (only works w/ Strings and "strings")
	operator StringIfHelperType() const { return buffer ? &string::StringIfHelper : 0; }
	int compareTo(const string &s) const;
	unsigned char equals(const string &s) const;
	unsigned char equals(const char *cstr) const;
	unsigned char operator == (const string &rhs) const {return equals(rhs);}
	unsigned char operator == (const char *cstr) const {return equals(cstr);}
	unsigned char operator != (const string &rhs) const {return !equals(rhs);}
	unsigned char operator != (const char *cstr) const {return !equals(cstr);}
	unsigned char operator <  (const string &rhs) const;
	unsigned char operator >  (const string &rhs) const;
	unsigned char operator <= (const string &rhs) const;
	unsigned char operator >= (const string &rhs) const;
	unsigned char equalsIgnoreCase(const string &s) const;
	unsigned char startsWith( const string &prefix) const;
	unsigned char startsWith(const string &prefix, unsigned int offset) const;
	unsigned char endsWith(const string &suffix) const;

	// character acccess
	char charAt(unsigned int index) const;
	void setCharAt(unsigned int index, char c);
	char operator [] (unsigned int index) const;
	char& operator [] (unsigned int index);
	void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index=0) const;
	void toCharArray(char *buf, unsigned int bufsize, unsigned int index=0) const
		{getBytes((unsigned char *)buf, bufsize, index);}
	const char * c_str() const { return buffer; }

	const char * to_str() const { 
		if (buffer) return buffer;
		else return "InvalideString"; 
	}

	// search
	int indexOf( char ch ) const;
	int indexOf( char ch, unsigned int fromIndex ) const;
	int indexOf( const string &str ) const;
	int indexOf( const string &str, unsigned int fromIndex ) const;
	int lastIndexOf( char ch ) const;
	int lastIndexOf( char ch, unsigned int fromIndex ) const;
	int lastIndexOf( const string &str ) const;
	int lastIndexOf( const string &str, unsigned int fromIndex ) const;
	string substring( unsigned int beginIndex ) const { return substring(beginIndex, len); };
	string substring( unsigned int beginIndex, unsigned int endIndex ) const;

	// modification
	void replace(char find, char replace);
	void replace(const string& find, const string& replace);
	void remove(unsigned int index);
	void remove(unsigned int index, unsigned int count);
//	void toLowerCase(void);
//	void toUpperCase(void);
//	void trim(void);

	// parsing/conversion
	long toInt(void) const;
	float toFloat(void) const;

	//gxx::const_buffer gbuf();

protected:
	char *buffer;	        // the actual char array
	unsigned int capacity;  // the array length minus one (for the '\0')
	unsigned int len;       // the string length (not counting the '\0')
protected:
	void init(void);
	void invalidate(void);
	unsigned char changeBuffer(unsigned int maxStrLen);
	unsigned char concat(const char *cstr, unsigned int length);

	// copy and move
	string & copy(const char *cstr, unsigned int length);
	//string & copy(const __FlashStringHelper *pstr, unsigned int length);
    void move(string &rhs);
};

class StringSumHelper : public string
{
public:
	StringSumHelper(const string &s) : string(s) {}
	StringSumHelper(const char *p) : string(p) {}
	StringSumHelper(char c) : string(c) {}
	StringSumHelper(unsigned char num) : string(num) {}
	StringSumHelper(int num) : string(num) {}
	StringSumHelper(unsigned int num) : string(num) {}
	StringSumHelper(long num) : string(num) {}
	StringSumHelper(unsigned long num) : string(num) {}
	StringSumHelper(float num) : string(num) {}
	StringSumHelper(double num) : string(num) {}
};

};

#endif  // String_class_h
