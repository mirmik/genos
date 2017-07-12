#ifndef DEBUG_WRITER
#define DEBUG_WRITER

#include <kernel/diag.h>
#include <gxx/debug/dprint.h>
#include <gxx/stream/writer.h>
#include <gxx/buffer.h>


class Debug : public genos::StreamWriter {
public:
	int putc(const char c) {
		return diag_putc(c);
	}

	int write(const char* str, size_t sz) {
		return diag_write(str, sz);
	}

	int room() {
		return 0xFFFF;
	}

	int dump(const gxx::buffer& buf) {
		debug_print_dump(buf.data(), buf.size());
	}
};

#include <util/ascii_convert.h>

class HDebug : public genos::StreamWriter {
public:
	int putc(const char c) {
		diag_putc(hbyte2hex((c & 0xF0) >> 4));
		diag_putc(hbyte2hex((c & 0x0F)));
		return 2;
	}

	int write(const char* str, size_t sz) {
		for (int i = 0; i < sz; i++) {
			putc(*str++);
		};
		return sz;
	}

	int room() {
		return 0xFFFF;
	}

	int dump(const gxx::buffer& buf) {
		debug_print_dump(buf.data(), buf.size());
	}
};

static inline Debug gDebug() {
	return Debug();
}

#endif