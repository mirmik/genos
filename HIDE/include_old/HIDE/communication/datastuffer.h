#ifndef GENOS_DATA_STUFFER_H
#define GENOS_DATA_STUFFER_H

#include <communication/stream.h>
#include <communication/sender.h>

struct DataStufferPrivate {
	char End;
	char Esc;
	char EscEnd;
	char EscEsc;
};

class OutputDataStuffer : public DataSender {
	DataStufferPrivate data;
	OutputDataStream * o;  
};

class InputDataStuffer {
	DataStufferPrivate data;
	InputDataStream * i;  
};

class DataStuffer : virtual public OutputStream, virtual public InputStream {

};

#endif