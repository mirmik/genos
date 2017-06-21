#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <gxx/io2/strm.h>
#include <resource/resource.h>

namespace genos {
	class file : public resource {
	public:
		virtual int write(const char* data, size_t size, size_t off) = 0;
		virtual int read(char* data, size_t size, size_t* off) = 0;

		virtual int seek(size_t off) = 0;
		//void resourceOpen() override;
		//void resourceRelease() override;
		//void resourceClose() override;
	};

	class lstrmio_file : public file {
	public:
		gxx::io::lstrmio& lstrm;
		lstrmio_file(gxx::io::lstrmio& lstrm) : lstrm(lstrm) {}

		int write(const char* data, size_t size, size_t off) {

		}

		int read(char* data, size_t size, size_t* off) {

		}

		int seek(size_t off) {

		}

		void resourceOpen() override {
			if (refs == 1) lstrm.clean();
		}

		void resourceRelease() override {}

		void resourceClose() override {

		}
	};
}

#endif