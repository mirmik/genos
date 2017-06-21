#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <gxx/io2/strm.h>
#include <resource/resource.h>
#include <sync/semaphore.h>
#include <sync/wait_queue.h>

#include <fcntl.h>

namespace genos {
	class file {
	public:
		uint16_t flags;

		virtual int write(const char* data, size_t size, size_t off) = 0;
		virtual int read(char* data, size_t size, size_t* off) = 0;
		virtual int seek(size_t off) { return -1; };
		virtual int pool() { return -1; };
		
		virtual int open() = 0;
		virtual int release() = 0;
		//void resourceOpen() override;
		//void resourceRelease() override;
		//void resourceClose() override;
	};

	class strmdev : public file {
	public:
		genos::semaphore wsem{1}, rsem{1};
		genos::wait_queue wwq, rwq;

		strmdev(){}

		int write(const char* data, size_t size, size_t off) override {
			return writeData(data, size);
		}

		int read(char* data, size_t size, size_t* off) override {
			int ret;

			if (rsem.down_interrutible()) {
				return EAGAIN;
			}

			if ((flags & O_NONBLOCK) && canRead() == 0) {
				rsem.up();
				return EAGAIN;
			}

			if (canRead() == 0) {
				if (schedee_wait_for(rwq)) {
					rsem.up();
					return EAGAIN;
				}
			}

			ret = readData(data, size);
			rsem.up();
			return ret;
		}

		int seek(size_t off) override {

		}

		int pool() override {

		}

		/*void resourceOpen() override {
			if (refs == 1) clean();
		}

		void resourceRelease() override {}

		void resourceClose() override {

		}*/

	protected:
		virtual int writeData(const char* data, size_t size) = 0;
		virtual int readData(char* data, size_t size) = 0;
		
		virtual int canRead() = 0;
		virtual int canWrite() = 0;
	
		virtual int clean();
	};

}

#endif