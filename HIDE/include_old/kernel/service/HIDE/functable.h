#ifndef GENOS_SERVICE_FUNCTION_TABLE_H
#define GENOS_SERVICE_FUNCTION_TABLE_H

#include <genos/compiler.h>

typedef int(*srvfnc_t)(ipcstack_t* stck);

struct service_function_table {
	const srvfnc_t* fnctbl;
	size_t 			tblsz;
}
typedef struct service_function_table srvbl_t;

__BEGIN_DECLS

void srvtbl_init(srvtbl_t* tbl, srvfnc_t* funcs, size_t sz);

static int srvtbl_invoke(srvtbl_t* tbl, size_t num, ipcstack_t* stck) {
	if 
}

__END_DECLS

#endif