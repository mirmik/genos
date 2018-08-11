#ifndef GENOS_PROCLIB_H
#define GENOS_PROCLIB_H

#include <gxx/event/delegate.h>

namespace genos {
	void sleep(int ms);
	void set_wait_handler(gxx::delegate<void>& dlg);
	void wait();
}

#endif