#ifndef GENOS_INVOKE_H
#define GENOS_INVOKE_H

//#include <gQ/gQMap.h>
#include <genos/sigslot/sigslot.h>

class InvokeSystem () {
	class Record {
		delegate<>
	};
};

/*GComFrame preinvoke(char* buf) {
	const char[50] name = ;
	if (!dictionary.contains(name)) return ErrorFrame(NONCONSIST); 
	ReturnFrame = invokeDictionary.at(name)(lalala);
};

template<typename Ret, typename Arg, Ret(*fn)(Arg)>
ReturnFrame invoke(name, lalala) {
	Arg arg = deserial(lalala);
	Ret ret = fn(arg);
	ReturnFrame rframe = serial(ret);
	return rframe;
}*/


#endif