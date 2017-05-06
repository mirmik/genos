#ifndef GENOS_DEBUG_STREAM_H
#define GENOS_DEBUG_STREAM_H

#include <debug/dprint.h>
#include <kernel/devices/serial/Stream.h>

namespace Genos {
	class DebugStream : public Stream {
	public:
		int write(const char* data, size_t size) {
			debug_write(data, size);
		}

		int read(char* data, size_t size) {
			//debug_read(data, size);
			return 0;
		}

		int avail() {
			return 0;
		}

		int room() {
			return 0xFFFF;
		}
	};

	namespace Glue {
		Stream& debugStream(); 
	}
}

#endif