#ifndef GENOS_IPC_QUERY_H
#define GENOS_IPC_QUERY_H

#include <kernel/id/id.h>
#include <kernel/ipcstack/ipcstack.h>
#include <gxx/HashTable.h>

class query {
public:
	dlist_head lnk;
	
	ipcstack* stack;
	fastaction callback; 
	
	id_t sender;	//info
	id_t receiver;	//info
};

class service {
public:
	hlist_node lnk;
	qid_t id;
	virtual void add_query(query*) = 0;

	service();

	//hashtable routine
	size_t hash(size_t seed) {
		return gxx::hash(id, seed);
	}

	id_t getkey() {
		return id;
	}
};

void kernel_registry_service(service* srv);
void kernel_service_hashtable_init();

extern gxx::HashTable<service, &service::lnk> service_hashtable;

#endif