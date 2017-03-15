#ifndef GENOS_KERNEL_SERVICE_HASHTABLE_H
#define GENOS_KERNEL_SERVICE_HASHTABLE_H

#include <kernel/service/service.h>

__BEGIN_DECLS

void kernel_service_table_init();
qid_t kernel_registrer_service(struct service* s);
void kernel_registrer_stable_service(struct service* s, qid_t qid);

struct service* kernel_get_service(qid_t qid);


__END_DECLS

#endif