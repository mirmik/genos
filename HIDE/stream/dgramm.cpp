#include <stream/dgramm.h>
#include <util/errbus.h>

inline void genos::dgrammStuffer::stuff(char c) {
	if (c == FEND) {
		outchar(FESC);
		outchar(TFEND);
		return;
	}
		
	if (c == FESC) {
		outchar(FESC);
		outchar(TFESC);
		return;
	}
		
	outchar(c);
} 

int genos::dgrammStuffer::send(const void* buf, int len, uint8_t flag) {
	const char* ptr = (const char*) buf;
	const char* end = (const char*) buf + len;

	outchar(FEND);
	while (ptr != end) {
		stuff(*ptr++);
	};
	outchar(FEND);
};

void genos::dgrammStuffer::endsymb() {
	if (rl.length() > 0) receivedMessage(rl.to_buffer());
	rl.init();
}


void genos::dgrammStuffer::__errorframe(ErrorBus err) {
	rl.init();
	errorFrame(err);
}

void genos::dgrammStuffer::unstuff(char c) {
	if (c == FESC) {
		lastsymb = FESC;
		return;
	}
		
	if (lastsymb == FESC) {
		lastsymb = c;
		switch(c) {
			case TFESC: 
				rl.putc(FESC); 
				return;
			case TFEND: 
				rl.putc(FEND); 
				return;
			default: 
				__errorframe(ErrorBus::WrongSymbolSequence);
				return;
		}
	}
		
	rl.putc(c);
} 

void genos::dgrammStuffer::inchar(char c) {
	if (c == FEND) {
		endsymb();
		return;
	};
	unstuff(c);	
};