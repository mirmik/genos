#ifndef GENOS_STD_STREAM_H
#define GENOS_STD_STREAM_H

#include <kernel/devices/serial/Stream.h>

namespace Genos {
	class Stream;
	
	struct StdStreamStruct {
		Genos::Stream * scout;
		Genos::Stream * scin;
	};

	extern StdStreamStruct * __stdstream; 

	void currentStdStream(StdStreamStruct * ptr);
	StdStreamStruct * currentStdStream();

	Stream& outStream();
	Stream& inStream();
}

#endif