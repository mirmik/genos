#ifndef UTILXX_BYTE_RING_H
#define UTILXX_BYTE_RING_H

#include <stdlib.h>

class ByteRing {

	//char* m_buf.data();	
	//size_t m_buf.size();
	gxx::buffer m_buf;

	size_t head;
	size_t tail;

	inline void fixup(size_t& ref) {
		while (ref >= m_buf.size()) ref -= m_buf.size(); 
	}

public:
	ByteRing(){};

	ByteRing(gxx::buffer buf) : m_buf(buf), head(0), tail(0) {};

	void init(char* buffer, size_t size) {
		m_buf.data(buffer);
		m_buf.size(size);
		head = tail = 0;
	}

	bool empty() volatile {
		return head == tail;
	}

	bool full() {
		return head == (tail ? tail : m_buf.size()) - 1;
	}

	int putc(char c) {
		if (full()) return 0;
		*(m_buf.data() + head++) = c;
		fixup(head);
		return 1;
	}
	
	int write(const char* data, size_t size) {
		int ret = 0;
		while(size--) {
			if(!putc(*data++)) {
				return ret;
			};
			ret++;
		}
		return ret;
	}

	int getc() {
		if (empty()) return -1;
		char c = *(m_buf.data() + tail++);
		fixup(tail);
		return c;
	}

	int read(char* data, size_t size) {
		int ret = 0;
		while(size--) {
			int r = getc();
			if (r == -1) {
				return ret;
			}
			*data++ = r;
			ret++;
		}
		return ret;
	}

	size_t avail() { 
		return (head >= tail) ? head - tail : m_buf.size() + head - tail; 
	};

	size_t room() {
		return (head >= tail) ? m_buf.size() - 1 + ( tail - head ) : ( tail - head ) - 1;
	};

	size_t size() {
		return m_buf.size();
	};
};

#endif