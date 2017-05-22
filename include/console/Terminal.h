#ifndef GENOS_TERMINAL_H
#define GENOS_TERMINAL_H

#include <kernel/devices/serial/FlagedStream.h>
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

		char c;

	protected:
		void destruct() override {
			dataWait.unbind();
		}

	public:
		bool debug_mode = false;
	
		void destroy() override {
			delete this;
		}

	public:
		Terminal(FlagedStream* strm, Stream* echo, Executor* executor, gxx::buffer buf) 
			: istrm(strm), ostrm(echo), dataWait(this), rl(buf), executor(executor) {}

		void lineHandler() {
			auto str = rl.line();
			//dprln(strlen(str));
			executor->execute(str);
			rl.reset();
		}

		Terminal& echo(bool en) {
			m_echo = en;
			return *this;
		}

		void routine() override {
			//dprln("TERMINAL");
			switch(state) {
				case 0:
					state++; 
				break;
				
				case 1:
					ostrm->print("input:");
					Genos::wakeup_wait(istrm->haveDataFlag, dataWait);
					//dprln("HERE");
					state++;
				break; 
				case 2: 
					while (istrm->avail()) {
						char lastsymb = c;
						c = istrm->getc();
						
						if (debug_mode) {
							ostrm->printhexln(c);
							continue;
						}

						if (c == '\r' && lastsymb == '\n') continue;

						if (c == '\n' || c == '\r') {
							state = 1;
							if (m_echo) ostrm->println();
							lineHandler();
							return;
						}

						//dprln(c);
						rl.putc(c);
						if (m_echo) ostrm->putc(c);
					}
					Genos::wakeup_wait(istrm->haveDataFlag, dataWait);
				break;
			}

			//dprln("Terminal");
		}
	};
}

#endif