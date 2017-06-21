#ifndef GENOS_LINER_SIGNAL_H
#define GENOS_LINER_SIGNAL_H

#include <stdio.h>

#undef putc

#include <genos/sigslot/sigslot.h>
#include <gxx/ReadLine.h>

class LinuxConsoleLiner {
private:
	gxx::ReadLine<64> rl;

public:

	void reader_func() {
		char c;
		char* ptr;
		while(1) {
			while(1) {
				c = getchar();
				if (c == '\n' || c == '\r') break;
				else rl.putc(c);
			}
			ptr = (char*)rl.c_str();
			if (rl.size() != 0) lineGetted(ptr);
			rl.init();
		}
	}

	gxx::signal<char*> lineGetted;
};

#endif