#ifndef GENOS_WREDIRECT
#define GENOS_WREDIRECT

#include "gxx/stream/writer.h"

namespace gxx {

class wredirect {
	writer* out;

	wredirect(writer* _out) : out(_out) {};

	int putc(const char c) {
		return out->putc(c);
	};

	int write(const char* str, size_t sz) {
		return out->write(str, sz);
	};

	int room() {
		return room();
	};
};

};

#endif