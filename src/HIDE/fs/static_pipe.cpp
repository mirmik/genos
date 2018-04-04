#include <fs/file.h>
#include <gxx/bytering.h>

#define PIPE_BUFFER_SIZE 64

namespace genos {
	class static_ring_pipe : public strmdev {
		char buf[PIPE_BUFFER_SIZE];
		gxx::bytering ring;

	public:
		static_ring_pipe() : ring(gxx::buffer(buf)) {}

		int writeData(const char* data, size_t size) {
			return ring.write(data, size);
		}

		int readData(char* data, size_t size) {
			return ring.read(data, size);
		}
		
		int canRead() {
			return ring.avail();
		}

		int canWrite() {
			return ring.room();
		}

		int clean() {

		}
	};

	int create_pipe() {

	}
}

