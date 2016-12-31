#ifndef GENOS_INPUT_STREAM_H
#define GENOS_INPUT_STREAM_H

#include <communication/ios.h>

class istream : public virtual ios {
public:
	istream(){
		dprln("InputDataStream");}
};

#include <communication/ostream.h>

class iostream : public virtual istream, public virtual ostream {
public:
	iostream(){
		dprln("IODataStream");}	
};

#endif