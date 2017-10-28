#ifndef GENOS_HISTORY_TERMINAL_H
#define GENOS_HISTORY_TERMINAL_H

#include <gxx/io/ostream.h>

namespace genos {
	class history_terminal {
		gxx::io::ostream& out;
		gxx::history hist;

	public:
		history_terminal(gxx::io::ostream& out, gxx::history&& hist) : out(out), hist(std::move(hist)) {}
	};
}

#endif