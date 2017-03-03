#include <kernel/service/table.h>

static void kernel_service_table_init() {
	service_hashtable.setStrategy(gxx::hash_memstrat70);
}

static void kernel_registry_srevice(service* s) {
	service_hashtable.put(*s);
}
