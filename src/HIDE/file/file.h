#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <gxx/io/base.h>

namespace genos {

	class chardev {
		gxx::event::rx_avail_flag;
		gxx::event::tx_empty_flag;

		int write(const char* buf, size_t size) = 0;
		int read(const char* buf, size_t size) = 0;

		int avail() = 0;
		int room() = 0;
	};

	/*struct file : public  {
	public:
		uint8_t flags;
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

	struct chardev_file : public file {
		genos::chardev_driver* drv;
		
		genos::semaphore osem;
		genos::semaphore isem;

		int write(const char* data, size_t size, size_t off) override {
			genos::down(&osem);
			
			//int writed = 0;
			//while(true) {
				//drv->write(data + writed, size - writed);
				//if (writed == size) break;
				//genos::wait_for_flag(&drv->tx_empty_flag);
			//}
			
			genos::up(&osem);
		};

		int read(char* data, size_t size, size_t* off) {
			size_t readed;

			genos::down(&isem);

			//if (drv->avail == 0) {
			//	genos::wait_for_flag(&drv->rx_avail_flag);	
			//}

			readed = drv->read(data, size);

			genos::up(&isem);	

			return readed;
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
	}*/
}

#endif