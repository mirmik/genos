#ifndef GENOS_TERMINAL_H
#define GENOS_TERMINAL_H

#include <kernel/devices/serial/Stream.h>
#include <kernel/sched/AutomSchedee.h>
#include <kernel/event/WakeUpWaiter.h>

#include <console/Executor.h>

#include <gxx/readline.h>

namespace Genos {
	class Terminal : public AutomFunctorSchedee {
		WakeUpWaiter dataWait;

		Stream* ostrm;
		FlagedStream* istrm;
		
		gxx::readline rl;

		Executor* executor;

		int state = 0;

		bool m_echo = true;
	public:
		Terminal(FlagedStream* strm, Stream* echo, Executor* executor, gxx::buffer buf) 
			: istrm(strm), ostrm(echo), dataWait(this), rl(buf), executor(executor) {}

		void lineHandler() {
			executor->execute(rl.line());
			rl.reset();
		}

		Terminal& echo(bool en) {
			m_echo = en;
			return *this;
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
						
						if (c == '\r') { return; }
						if (c == '\n') {
							state = 1;
							if (m_echo) ostrm->putc('\n');
							lineHandler();
							return;
						}

						rl.putc(c);
						if (m_echo) ostrm->putc(c);
					}
					Genos::wait(istrm->haveDataFlag, dataWait);
				break;
			}
			//dprln("Terminal");
		}
	};
}

#endif