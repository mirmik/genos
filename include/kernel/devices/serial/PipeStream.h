#ifndef GENOS_PIPE_STREAM_H
#define GENOS_PIPE_STREAM_H

#include <utilxx/classes/ByteRing.h>
#include <kernel/devices/serial/Stream.h>
#include <gxx/buffer.h>

namespace Genos {

	class PipeStream : public FlagedStream {
		
		ByteRing ring;

	public:
		PipeStream(gxx::buffer buf) : ring(buf) {}

		int write(const char* data, size_t size) override {
			auto ret = ring.write(data, size);
			haveDataFlag.set();
			return ret;
		}

		int read(char* str, size_t len) override {
			auto ret = ring.read(str, len);
			if (ring.avail() == 0) haveDataFlag.reset(); 
			return ret;
		}

		int avail() override {
			return ring.avail();
		}
	
		int room() override {
			return ring.room();
		}

	};

}

#endif