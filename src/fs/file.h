#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <gxx/io/base.h>

namespace genos {
	struct file : public  {
	public:
		virtual int write(const char* data, size_t size, size_t off) = 0;
		virtual int read(char* data, size_t size, size_t* off) = 0;
		//virtual int seek(size_t off) { return -1; };
		//virtual int pool() { return -1; };

		virtual void open() {};
		virtual void release() {};
		
		//void resourceOpen() override;
		//void resourceRelease() override;
		//void resourceClose() override;
	};

	struct iostorage_chardev : public file {
		gxx::iostorage* storage;
		genos::wait_queue txwait;

		uint8_t refs = 0;

		int write(const char* data, size_t size, size_t off) override {
			genos::wait_head waiter;
				
			genos::csection_enter();
			if (!txwait.empty()) {
				genos::init_waiter_for_current_schedee(&waiter);
				txwait.add(&waiter);
				__displace__();
			}

			int writed = 0;
			while(true) {
				writed += storage->write(data + writed, size - writed);
				if (writed == size) break;
				genos::init_waiter_for_current_schedee(&waiter);
				txwait.add_priority(&waiter);
				__displace__();
			}
			genos::csection_leave();
		};

		int read(char* data, size_t size, size_t* off) {

		};

		void open() {
			genos::csection_guard guard; 
			if (refs == 0) storage->clean; 
			++refs;
		};

		void release() {
			genos::csection_guard guard; 
			--refs;
		}
	}
}

#endif