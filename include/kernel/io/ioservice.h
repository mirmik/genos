#ifndef GENOS_IOSERVICE_H
#define GENOS_IOSERVICE_H

#include <gxx/buffer.h>

typedef void(*srvfunc_t)(struct query* q); 

class MultiFunctionService : public service {
	gxx::buffer<srvfunc_t> funcs;
}

#endif