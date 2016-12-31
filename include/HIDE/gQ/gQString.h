#ifndef GENOS_GQ_STRING_H
#define GENOS_GQ_STRING_H

#include <gQ/gQByteAllocated.h>
#include <gQ/gQTraits.h>
#include <gxx/utility.h>
#include <gxx/algorithm.h>
#include <debug/dprint.h>

#include <string.h>
#include <assert.h>

class gQString : public gQByteAllocated {
private:
	char* m_data;
	size_t m_size;
	size_t m_capacity;

public:
	//gQString() gQByteAllocated() {
	//	init();
	//};

	gQString() : gQByteAllocated() {
		init();
	}


	explicit gQString(const char* cstr) : gQByteAllocated() {
		init();
		copy(cstr, strlen(cstr));
	};


private:

	inline void init(void) {
		m_data = nullptr;
		m_capacity = 0;
		m_size = 0;	
	}

	size_t memoryStrategy(size_t need) {
		return need;
	};

	void invalidate(void) {
		if (m_data) m_alloc->deallocate(m_data);
		m_data = nullptr;
		m_capacity = m_size = 0;
	}

	unsigned char reserve(size_t sz) {
		if (sz <= m_capacity) return 1;
		size_t need = memoryStrategy(sz);
		return changeBuffer(need);
	};

	unsigned char changeBuffer(size_t maxStrLen) {
		char *newbuffer = (char *)m_alloc->reallocate(m_data, maxStrLen + 1);
		if (newbuffer) {
			m_data = newbuffer;
			m_capacity = maxStrLen;
			return 1;
		}
		return 0;
	}

	gQString & copy(const char *cstr, unsigned int length) {
		if (!reserve(length)) {
			invalidate();
			return *this;
		}
		m_size = length;
		memcpy(m_data, cstr, length);
		return *this;
	}

public:

	const char* c_str() {
		*(m_data + m_size) = 0;
		return m_data;
	}

	gQString & operator = (const char *cstr)
	{
		if (cstr) copy(cstr, strlen(cstr));
		else invalidate();
	
		return *this;
	}
	 
};

#endif