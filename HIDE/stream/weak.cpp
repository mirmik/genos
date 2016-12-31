#include <protocol/weak.h>
#include <crypto/crc.h>
#include <debug/dwriter.h>

inline void Wake::stuff(char c) {
	if (c == FEND) {
		bus->putc(FESC);
		bus->putc(TFEND);
		return;
	}
		
	if (c == FESC) {
		bus->putc(FESC);
		bus->putc(TFESC);
		return;
	}
		
	bus->putc(c);
}

int Wake::sendFrame(Wake::Frame& frame) {
	
	Crc8Functor crc8f;
	const char* end = (const char*) frame.data + frame.len;
	const char* ptr;

	crc8f(FEND);
	crc8f(frame.sender);
	crc8f(frame.recver);
	crc8f(frame.cmd);
	crc8f(frame.len);
	
	ptr = (const char*) frame.data;
	while (ptr != end) {
		crc8f(*ptr++);
	}

	bus->putc(FEND);
	stuff(frame.sender);
	stuff(frame.recver);
	stuff(frame.cmd);
	stuff(frame.len);
	
	ptr = (const char*) frame.data;
	while (ptr != end) {
		stuff(*ptr++);
	};	

	bus->putc(crc8f);
}

void Wake::error_unfinished_frame() {

}

void Wake::newc(char c) {
	switch(status) {
		case STATUS_WAIT_STRT:
			if (c != FEND) return;
			status = STATUS_WAIT_CHAR; 
			break;
		case STATUS_WAIT_CHAR:
			if (c == FEND) 
	}	
}