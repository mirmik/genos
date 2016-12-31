#ifndef GENOS_SERIAL_INVOKER_H
#define GENOS_SERIAL_INVOKER_H

#include <gQ/gQSerialBuffer.h>
//#include <gxx/container/dictionary.h>

class SerialInvoker {

	struct Record {
		void* func;
		void (*invoker) (gQSerialBuffer&, const gQSerialBuffer&);
	};

	//dictionary<>

};

template <typename Ret, typename Arg>
void invokeSerial(void* fn, gQSerialBuffer& outbuf, const gQSerialBuffer& inbuf) {
	DeserialBuffer debuf = inbuf.toDeserialBuffer();
	//char name[128];
	Arg arg;
	//debuf.deserialization(gxx::buffer(name,0));
	debuf.deserialization(arg);
		
	dprln(arg);

	Ret ret = ((Ret(*)(Arg))fn)(arg);
	//outbuf.reserve_back(10);
	outbuf.pushserial_back(ret);
}

#endif