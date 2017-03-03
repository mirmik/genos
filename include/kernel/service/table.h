#ifndef GENOS_KERNEL_SERVICE_HASHTABLE_H
#define GENOS_KERNEL_SERVICE_HASHTABLE_H

#include <gxx/HashTable.h>
#include <kernel/service/service.h>

extern gxx::HashTable<service, &service::hlnk> service_hashtable;

__BEGIN_DECLS

qid_t kernel_get_new_qid();

void kernel_service_table_init();
qid_t kernel_registry_service(service* s);
void kernel_registry_stable_service(service* s, qid_t qid);

struct service* kernel_get_service(qid_t qid);


__END_DECLS

#endif