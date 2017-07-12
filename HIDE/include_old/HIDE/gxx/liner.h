#ifndef GENOS_LINER_H
#define GENOS_LINER_H

#include <genos/defines/size_t.h>
#include <gxx/buffer.h>

namespace gxx {

class liner : public writer {

	gxx::allocated_buffer buf;
	
	size_t capacity;
	size_t cursor;

public:

	liner() : buf(), cursor(0) {};

	void create_buffer(size_t sz) {
	}

	void shrink_buffer() {
	}

	gxx::buffer unlink_buffer() {
	}

	int putc(const char c) {
		//assert(buf.is_valid());
		if (room() <= 0) return 0;
		buf.data[cursor++] = c;
		return 1;
	}
	
	int write(const char* str, size_t len) {
		//assert(buf.is_valid());
		size_t __room = room();
		len = min(len, __room);
		memcpy(buf.data + cursor, str, len);
		cursor = cursor + len;
		return len;
	}

	int room() {
		return buf.size - cursor;
	}
};

}

#endif