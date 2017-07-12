#ifndef GENOS_MVECTOR_H
#define GENOS_MVECTOR_H

//#include "defines/size_t.h"
//#include "util/assert.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include <utilxx/new.h>
#include "utilxx/to_string.h"


template <typename type>
class mvector
{
private:
	type* buffer;
	size_t capacity;
	size_t len;

public:
	mvector()
	{
		init();
	};

	~mvector()
	{
		overall_destructor();
		free(buffer);
	};

	void overall_destructor()
	{
		for(type* ptr = buffer; ptr != end(); ++ptr)
			destructor(ptr);
	};

	void destructor(type* ptr)
	{
		ptr -> type::~type();
	};

	mvector(const mvector &rhs)
	{
		init();
		if (rhs.buffer) construct_copy(rhs.buffer, rhs.len);
		else invalidate();
	}
	
	mvector(mvector &&rval)
	{
		init();
		move(rval);
	}

	unsigned char push_back(const type& value)
	{
		if(!reserve(len + 1)) return 0;
		new (buffer + len) type(value);
		++len;
		return 1;
	}

	unsigned char push_front(const type& value)
	{
		if(!reserve(len + 1)) return 0;
		shiftPart_right(buffer, buffer + len, 1);
		new (buffer) type(value);
		++len;
		return 1;
	}
/*
	unsigned char push_back(type&& value)
	{
		if(!reserve(len + 1)) return 0;
		new (buffer + len) type(value);
		++len;
		return 1;
	}

	unsigned char push_front(type&& value)
	{
		if(!reserve(len + 1)) return 0;
		shiftPart_right(buffer, buffer + len, 1);
		new (buffer) type(value);
		++len;
		return 1;
	}
*/
	unsigned char insert(const type& value, size_t i)
	{
		assert(i <= len);
		if(!reserve(len + 1)) return 0;;
		shiftPart_right(buffer+i, buffer+len, 1);
		new (buffer + i) type(value);
		++len;
		return 1;
	};


	unsigned char remove(type* obj)
	{
		assert(obj >= buffer && obj < buffer + len);
		destructor(obj);
		shiftPart_left(obj + 1, buffer + len, 1);
		--len;
		return 1;
	};

	unsigned char remove(size_t i)
	{
		remove(buffer + i);
	};


	type& operator[](int i)
	{
		assert(i < capacity);
		return *(buffer + i);
	};

	void init()
	{
		buffer 		= nullptr;
		capacity 	= 0;
		len 		= 0;
	};

	void move(mvector &rhs)
	{
		if (buffer) {overall_destructor(); free(buffer);}
		buffer = rhs.buffer;
		capacity = rhs.capacity;
		len = rhs.len;
		rhs.buffer = nullptr;
		rhs.capacity = 0;
		rhs.len = 0;
	}

	mvector & construct_copy(const type *ptr, size_t length)
	{
		if (!reserve(length)) {
			invalidate();
			return *this;
		}
		len = length;
		for (int i = 0; i < length; ++i) 
			new (buffer + i) type(*(ptr + i)); 
		return *this;
	}

	mvector & copy(const type *ptr, size_t length)
	{
		if (!reserve(length)) {
			invalidate();
			return *this;
		}
		len = length;
		for (int i = 0; i < length; ++i) 
			*(buffer + i) = *(ptr + i); 
		return *this;
	}

	type * begin()
	{
		return buffer; 
	};

	type * end()
	{
		return buffer + len;
	};

	unsigned char size(size_t i)
	{
		if (reserve(i)) 
			{
				len = i;
				return 1;
			};
		return 0;
	}

	unsigned char reserve(size_t size)
	{
		if (buffer && capacity >= size) return 1;
		if (changeBuffer(size)) {
			return 1;
		}
		return 0;
	}

	unsigned char shiftPart_right(type* begin, type* end, size_t bias)
	{
		type *src = end;
		type *dst = end + bias;
		while(src != begin)
			{
				new (--dst) type(gxx::move(*--src));
			};
		return 1;
	}	

	unsigned char shiftPart_left(type* begin, type* end, size_t bias)
	{
		type *src = begin;
		type *dst = begin - bias;
		while(src != end)
			{
				new (dst++) type(gxx::move(*src++));
			};
		return 1;
	}	

	unsigned char changeBuffer(size_t size)
	{
		//type *newbuffer = (type *)realloc(buffer, size  * sizeof(type));
		type *newbuffer = (type*) malloc(size * sizeof(type));
		for (int i = 0; i < len; ++i)
		{
			new (newbuffer + i) type(gxx::move(*(buffer + i)));
		};
		free (buffer);
		if (newbuffer) {
			buffer = newbuffer;
			capacity = size;
			return 1;
		}
		return 0;
	}

	void invalidate(void)
	{
		if (buffer) 
		{
			overall_destructor();
			free(buffer);
		};
		buffer = NULL;
		capacity = len = 0;
	}

	mvector & operator = (const mvector &rhs)
	{
		if (this == &rhs) return *this;
	
		overall_destructor();
		if (rhs.buffer) construct_copy(rhs.buffer, rhs.len);
		else invalidate();
		
		return *this;
	}

	type* to_array()
	{
		return buffer;
	}

	size_t length() const
	{
		return len;
	};

	gxx::string to_info()
	{
		gxx::string str;
		str.reserve(128);
		str 
		<< "\r\nmvector:" 
		<< "\r\nlen: " << len 
		<< "\r\ncapacity: " << capacity 
		<< "\r\n";
		return str;
	}

	gxx::string to_str()
	{
		gxx::string str;
		str << "[";
		for (int i = 0; i < len; ++i)
			{
				str << ::to_str(*(buffer + i));
				if (i != len - 1) str << ',';
			};
		str << "]";
		return str;
	};

	mvector & operator << (const type & value)
	{
		push_back(value);
		return *this;
	};
};

#endif