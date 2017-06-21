#ifndef GENOS_RECEIVER_H
#define GENOS_RECEIVER_H

namespace genos {
	
	class Read {
		virtual int read(const void* str, size_t len) = 0;

		virtual int getchar() {
			char c;
			int ret = read(c, 1);
			return ret < 0 ? ret : c;
		}

		virtual int avail() = 0;

		virtual int free() = 0;
	};

}

#endif