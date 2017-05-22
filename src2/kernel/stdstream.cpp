#include <kernel/stdstream.h>

namespace Genos {

	StdStreamStruct * __stdstream; 
	
	void currentStdStream(StdStreamStruct * ptr) { __stdstream = ptr; }
	StdStreamStruct * currentStdStream() { return __stdstream; }
	
	Stream& outStream() { return *__stdstream->scout; }
	Stream& inStream() { return *__stdstream->scin; }

}