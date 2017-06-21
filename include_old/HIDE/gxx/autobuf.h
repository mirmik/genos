#ifndef GENOS_AUTOBUF
#define GENOS_AUTOBUF

#include "mem/sysalloc.h"
#include <defines/atomic_t.h>
#include <gxx/buffer.h>

namespace gxx {

struct core_autobuf : gxx::buffer {
	atomic_t refs;
};

class autobuf {
	core_autobuf* ab;

public:
	autobuf(size_t length) {
		ab = (core_autobuf*) sysalloc(sizeof(core_autobuf));
		ab->allocate(sysalloc, length);
		ab->refs = 1;
	}

	autobuf(const autobuf& other) {
		ab = other.ab;
		ab->refs++;
	}

//	autobuf(autobuf&& other) {
//		dprln("move");
//		ab = other.ab;
//		ab->refs++;
//	}

	~autobuf() {
		//TODO lock
		ab->refs--;
		if (ab->refs == 0) {
			ab->deallocate(sysfree);
			sysfree(ab);
		};
	}

	int resize (size_t newsz) {
		//ab->buffer = sysrealloc(ab->data, newsz);
	};

	gxx::buffer get_buffer() {
		return *ab;
	}
};

};

#endif