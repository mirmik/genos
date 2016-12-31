#ifndef MSG_SEND_H
#define MSG_SEND_H

#include "inttypes.h"

class MsgSend {
		/*	
		send - Передать сообщение.
		@buf : буффер-источник
		@len : длина сообщения
		@flag : опции
		@return : код возврата. Отрицательный в случае ошибки. 
	*/
	virtual int send(const void* buf, int len, uint8_t flag = 0) = 0;
};

#endif