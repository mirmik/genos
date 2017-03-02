#include <kernel/service/service.h>
#include <kernel/service/table.h>

gxx::HashTable<service, &service::hlnk> service_hashtable;

struct service* get_service(qid_t qid) {
	service* s;
	bool res = service_hashtable.get(qid, s);
	return res ? s : NULL;
}