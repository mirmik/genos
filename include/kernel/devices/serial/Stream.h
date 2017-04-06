#ifndef GENOS_STREAM
#define GENOS_STREAM

#include <kernel/event/Tasklet.h>
#include <kernel/devices/serial/Print.h>

namespace Genos {

	class Stream : public Print {
	
	public:
		virtual int read(char* str, size_t len) = 0;
	
		char getc() {
			char c;
			int ret = read(&c, 1);
			if (ret != 1) return -1;
			return c;	
		}
	
		virtual int avail() = 0;
		virtual int room() = 0;
	
	};
	
	class FlagedStream : public Stream {
	public:
		StateFlag haveDataFlag;
	
	//public:
		//void waitReceivedData(Waiter& waiter) {
		//	haveDataFlag.wait(waiter);
		//}
	};

	class DebugStream : public Stream {
	public:
		int write(const char* data, size_t size) {
			debug_write(data, size);
		}

		int read(char* data, size_t size) {
			//debug_read(data, size);
		}

		int avail() {
			return 0;
		}

		int room() {
			return 0xFFFF;
		}
	};

}

#endif