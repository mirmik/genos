#ifndef PROTOCOL_WEAK_H
#define PROTOCOL_WEAK_H

#include <stream/msgsend.h>
#include <stream/msgrecv.h>
#include <gxx/debug/dprint.h>
#include <gxx/stream/writer.h>
#include <gxx/readline.h>

class Wake {

	static constexpr char FEND = 0xC0; 	/*Frame End*/ 	
	static constexpr char FESC = 0xDB; 	/*Frame Escape*/
	static constexpr char TFEND = 0xDC; 	/*Transposed Frame End*/
	static constexpr char TFESC = 0xDD;	/*Transposed Frame Escape*/

	static constexpr char STATUS_WAIT_STRT = 0xFF;
	static constexpr char STATUS_WAIT_SYMB = 0x00;
	static constexpr char STATUS_WAIT_CODE = 0x01;
	static constexpr char STATUS_ERROR = 0x02;

public:

	Wake() {
		status = STATUS_WAIT_STRT;
		rl.init();
	}

	struct Frame {
		uint8_t addr;
		uint8_t cmd;
		const void* data;
		uint8_t len;
		uint8_t crc;
	};

	genos::StreamWriter* bus;
	ReadLine<128> rl;
	uint8_t status;

	void stuff(char c);
	int sendFrame(Wake::Frame& frame);

	void newc(char c);

};


#endif