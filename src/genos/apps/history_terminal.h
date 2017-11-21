#ifndef GENOS_HISTORY_TERMINAL_H
#define GENOS_HISTORY_TERMINAL_H

#include <gxx/io/ostream.h>
#include <gxx/io/iostorage.h>
#include <gxx/history.h>

namespace genos {
	class history_terminal {
		gxx::io::ostream& out;
		gxx::io::istorage& in;
		gxx::history hist;
		gxx::readline rl;

		bool echo = true;

	public:
		history_terminal(gxx::io::ostream& out, gxx::history&& hist) : out(out), hist(std::move(hist)) {}

		void newline() {
			dprln("newline");
		}

		void execute() {
			while(in.avail()) {
				char c = in.getchar();

				switch (c) {
					case '\r' : continue;
					case '\n' : newline();
					default:
						if (echo) out.putchar(c);
						rl.putchar(c);
				}
			}
			return;
		}
	};
}

#endif