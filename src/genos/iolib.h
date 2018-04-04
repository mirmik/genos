#ifndef GENOS_IOLIB_H
#define GENOS_IOLIB_H

namespace genos {

	static inline void writeall(genos::file* flp, const char* data, size_t size, size_t off = 0) {
		if (flp->is_cstream()) {
			auto cstream = static_cast<genos::cstream*>(flp);
			size_t writed = 0;
			while(writed != size) {
				writed += flp->write(data + writed, size - writed, off + writed);
				while(cstream->room() == 0);
			}
		}
		else {
			flp->write(data, size, off);
		}
	}

	static inline void print(genos::file* flp, const char* data) {
		writeall(flp, data, strlen(data));
	}

}

#endif