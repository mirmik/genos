#ifndef GENOS_TERMINAL_H
#define GENOS_TERMINAL_H

#include <kernel/devices/serial/Stream.h>
#include <kernel/sched/AutomSchedee.h>
#include <kernel/event/WakeUpWaiter.h>

#include <gxx/readline.h>

namespace Genos {
	class Terminal : public AutomFunctorSchedee {
		WakeUpWaiter dataWait;

		Stream* ostrm;
		FlagedStream* istrm;
		
		gxx::readline rl;

		int state = 0;
	public:
		Terminal(FlagedStream* strm, Stream* echo, gxx::buffer buf) 
			: istrm(strm), ostrm(echo), dataWait(this), rl(buf) {}

		void lineHandler() {
			ostrm->println("LineReceived");
			rl.reset();
		}

		void routine() override {
			switch(state) {
				case 0:
					state++; 
				break;
				
				case 1:
					ostrm->print("input:");
					Genos::wait(istrm->haveDataFlag, dataWait);
					state++;
				break; 
				case 2: 
					while (istrm->avail()) {
						char c = istrm->getc();
						rl.putc(c);

						if (c == '\n') {
							state = 1;
							ostrm->putc('\n');
							lineHandler();
							break;
						}

						ostrm->putc(c);
					}
					Genos::wait(istrm->haveDataFlag, dataWait);
				break;
			}
			//dprln("Terminal");
		}
	};
}

#endif