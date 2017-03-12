#include <kernel/service/table.h>

void kernel_service_table_init() {
	service_hashtable.setStrategy(gxx::hash_memstrat70);
}

void kernel_registry_service(service* s) {
	service_hashtable.put(*s);
}
