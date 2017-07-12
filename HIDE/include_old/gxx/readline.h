#ifndef GXX_READLINE_H
#define GXX_READLINE_H

//#include <genos/defines/size_t.h>
//#include <gxx/algorithm.h>
#include <gxx/buffer.h>
#include <stdlib.h>

namespace gxx {
	class readline {
		char* m_data;
		size_t m_size;
		
		size_t cursor;

	public:
		readline(gxx::buffer buf) : 
			m_data(buf.data()), m_size(buf.size()), cursor(0) 
		{}

		bool is_full() {
			return cursor == m_size - 1;
		}

		void reset() {
			cursor = 0;
		}

		int putc(unsigned char c) {
			if (is_full()) return 0;
			m_data[cursor++] = c;
			return 1;
		}

		size_t size() const {
			return cursor;
		}

		//size_t copy_to(char* str, size_t bufsz) {
		//	auto len = min(bufsz, cursor);
		//	memcpy(str, line, len);
		//	return len;
		//}

		const char* data() const {
			return m_data;
		}

		char* line() {
			m_data[cursor] = 0;
			return m_data;
		}

		//char* c_str_noconst() {
		//	line[cursor] = 0;
		//	return line;
		//};

		gxx::buffer to_buffer() {
			return gxx::buffer(m_data, cursor);
		}
	};
}

#endif