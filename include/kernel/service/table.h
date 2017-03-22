#ifndef GENOS_KERNEL_SERVICE_HASHTABLE_H
#define GENOS_KERNEL_SERVICE_HASHTABLE_H

#include <kernel/service/service.h>

//__BEGIN_DECLS

namespace genos {

	void service_table_init();
	qid_t register_service(genos::service* s);
	void register_stable_service(genos::service* s, qid_t qid);

	genos::service* get_service(qid_t qid);

}

//__END_DECLS

#endif