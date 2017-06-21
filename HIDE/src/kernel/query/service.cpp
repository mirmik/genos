#include <kernel/ipcstack/query.h>

static id_t __current_id = 1;

id_t get_service_id() {
	return __current_id++;
}

service::service() {}

void kernel_registry_service(service* srv) {
	srv->id = get_service_id();
	service_hashtable.put(*srv);
}


gxx::HashTable<service, &service::lnk> service_hashtable;

void kernel_service_hashtable_init() {
	service_hashtable.setStrategy(gxx::hash_memstrat70);
}
