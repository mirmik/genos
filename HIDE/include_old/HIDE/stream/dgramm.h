#ifndef GENOS_DGRAM_PROTOCOL
#define GENOS_DGRAM_PROTOCOL

#include <stream/messanger.h>
#include <genos/sigslot/sigslot.h>
#include <gxx/readline.h>

namespace genos {

class dgrammStuffer : public Messanger {
public:

	static constexpr char FEND = 0xC0; 	/*Frame End*/ 	
	static constexpr char FESC = 0xDB; 	/*Frame Escape*/
	static constexpr char TFEND = 0xDC; 	/*Transposed Frame End*/
	static constexpr char TFESC = 0xDD;	/*Transposed Frame Escape*/

	gxx::ReadLine<128> rl;
	char lastsymb;

public:	
	int send(const void* buf, int len, uint8_t flag = 0);

	signal<char> outchar;

	inline void stuff(char c);
	inline void unstuff(char c);

	void inchar(char c);


private:
	void __errorframe(ErrorBus err);	
	void endsymb();
};

};

#endif