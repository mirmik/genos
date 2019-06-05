#ifndef GENOS_LINER_H
#define GENOS_LINER_H

#include <drivers/tty/ldisc.h>

namespace genos 
{
	class liner_discipline : public ldisc 
	{
		struct readline rl;
		char * buffer;
		int cursor;
		int newline_index = -1;

	public:
		void recvbyte_handler(char c) override 
		{
			if (c == '\r') 
				return;

			if (c == '\n' && newline_index == -1) 
				newline_index = cursor;

			buffer[cursor++];
		}
	};
}

#endif