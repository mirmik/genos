#ifndef GENOS_LINUX_TERMINAL_H
#define GENOS_LINUX_TERMINAL_H

#include <stdio.h>
#include <unistd.h>

#include <kernel/devices/serial/Stream.h>
#include <kernel/csection.h>

#include <thread>

#include <gxx/array.h>
#include <utilxx/classes/ByteRing.h>

namespace Genos { 

	class LinuxTerminal : public FlagedStream {
		gxx::array <char, 1024> arr;
		ByteRing rx;

		std::thread readThread;

		void readFunc() {
			while(1) {
				char c = getchar();
				//dprln("newchar");
				atomic_section_enter();
				rx.putc(c);
				haveDataFlag.set();
				atomic_section_leave();
			}
		}

	public:
		LinuxTerminal() : rx(arr.slice()), readThread(&LinuxTerminal::readFunc, this) {}

		~LinuxTerminal() { readThread.detach(); }

		int write(const char* data, size_t size) {
			//atomic_section_enter();
			auto ret = ::write(1, data, size);
			fflush(stdout);
			//atomic_section_leave();
			return ret;
		}
		
		int read(char* data, size_t size) {
			atomic_section_enter();
			auto ret = rx.read(data, size);
			if (rx.avail() == 0) haveDataFlag.reset();
			atomic_section_leave();
			return ret;
		}
	
		int avail() { 
			return rx.avail(); 
		}
		
		int room() { 
			return 0xFFFF; 
		}
	
		int flush() {
			return 0;
		}
	};

}

#endif