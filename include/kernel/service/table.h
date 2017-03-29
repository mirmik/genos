#ifndef GENOS_KERNEL_SERVICE_HASHTABLE_H
#define GENOS_KERNEL_SERVICE_HASHTABLE_H

#include <kernel/service/service.h>

//__BEGIN_DECLS

namespace Genos {

	void serviceTableInit();
	
	qid_t register_service(Service* s);
	void register_stable_service(Service* s, qid_t qid);

	Service* get_service(qid_t qid);

}

//__END_DECLS

#endif