#ifndef GENOS_GPRINT_H
#define GENOS_GPRINT_H

#include <kernel/devices/serial/Print.h>

namespace Genos { namespace print {

	Print* stdout = nullptr;

	template <typename ... Args>
	int print(Args ... args) {
		assert(stdout);
		stdout->print(gxx::forward<Args>(args) ...);
	}

	template <typename ... Args>
	int println(Args ... args) {
		assert(stdout);
		stdout->println(gxx::forward<Args>(args) ...);
	}

	template <typename ... Args>
	int printhex(Args ... args) {
		assert(stdout);
		stdout->printhex(gxx::forward<Args>(args) ...);
	}

	template <typename ... Args>
	int printhexln(Args ... args) {
		assert(stdout);
		stdout->printhexln(gxx::forward<Args>(args) ...);
	}

}}

#endif