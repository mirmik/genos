#ifndef STREAM_MESSANGER_H
#define STREAM_MESSANGER_H

#include "inttypes.h"
#include <genos/sigslot/sigslot.h>
#include <gxx/buffer.h>
#include <util/errbus.h>

class Messanger {
	/*	
		send - Передать сообщение.
		@buf : буффер-источник
		@len : длина сообщения
		@flag : опции
		@return : код возврата. Отрицательный в случае ошибки. 
	*/
	virtual int send(const void* buf, int len, uint8_t flag = 0) = 0;
	
public:
	genos::signal<gxx::buffer> receivedMessage;
	genos::signal<ErrorBus> errorFrame;
};

#endif