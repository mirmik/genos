#ifndef GXX_READLINE_H
#define GXX_READLINE_H

#include <genos/defines/size_t.h>
#include <gxx/algorithm.h>
#include <gxx/buffer.h>

namespace gxx {

	template <size_t N>
	class ReadLine {
		char line[N];
		size_t cursor;

	public:
		void init() {
			cursor = 0;
		};

		bool is_full() {
			return cursor == N;
		}

		int putc(unsigned char c) {
			if (is_full()) return 0;
			line[cursor++] = c;
		}

		size_t size() const {
			return cursor;
		}

		size_t copy_to(char* str, size_t bufsz) {
			auto len = min(bufsz, cursor);
			memcpy(str, line, len);
			return len;
		}

		const char* data() const {
			return line;
		};

		const char* c_str() {
			line[cursor] = 0;
			return line;
		};

		char* c_str_noconst() {
			line[cursor] = 0;
			return line;
		};

		gxx::buffer to_buffer() {
			return gxx::buffer(line, cursor);
		}

		ReadLine() : cursor(0) {};
	};
}

#endif