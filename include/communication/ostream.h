#ifndef GENOS_OUTPUT_STREAM_H
#define GENOS_OUTPUT_STREAM_H

#include <communication/ios.h>

class ostream : public virtual ios {
public:
	ostream(){
		dprln("OutputDataStream");
	}
};

#endif