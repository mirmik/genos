#ifndef GENOS_KERNEL_SERVICE_HASHTABLE_H
#define GENOS_KERNEL_SERVICE_HASHTABLE_H

#include <gxx/HashTable.h>
#include <kernel/service/service.h>

extern gxx::HashTable<service, &service::hlnk> service_hashtable;

static void kernel_service_table_init() {
	service_hashtable.setStrategy(gxx::hash_memstrat70);
}

#endif