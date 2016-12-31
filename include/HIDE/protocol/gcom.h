#ifndef GENOS_PROTOCOL_GCOM_H
#define GENOS_PROTOCOL_GCOM_H

#include <stream/msgsend.h>
#include <gQ/gQByteArray.h>
#include <gQ/gQSerialBuffer.h>
//#include <gxx/hashtable.h>

//cs 			2
//proto 		1
//code			1
//datalength	2
//data 			var

class gCom {
	Messanger* m_sender;

public:
	gCom(Messanger* sender) : m_sender(sender) {};

	void send(uint8_t proto, uint8_t code, gQSKB buffer);
		
};

#endif