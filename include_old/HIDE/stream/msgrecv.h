#ifndef MSG_RECV_H
#define MSG_RECV_H

#include "inttypes.h"

class MsgRecv {
	/*	
		recv - Принять сообщение.
		@buf : буффер-приёмник
		@max : максимальная длина принимаемого сообщения
		@flag : опции
		@return : код возврата. Отрицательный в случае ошибки. 
	*/
	virtual int recv(const void* buf, int max, uint8_t flag = 0) = 0;
};

#endif