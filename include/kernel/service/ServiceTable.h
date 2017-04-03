#ifndef GENOS_SERVICE_TABLE_H
#define GENOS_SERVICE_TABLE_H

#include <kernel/service/Service.h>
#include <gxx/hashtable.h>

#define SERVICE_HASHTABLE_SIZE 20
#define MAX_QID 1000

//int& __service_getkey (Genos::Service& srvs) { return srvs.qid; }

namespace Genos {

	class ServiceTable {

		qid_t qid_counter;
		gxx::static_hashtable<SERVICE_HASHTABLE_SIZE,
			Service, qid_t, &Service::hlnk> table;

	public:
		ServiceTable() {}

	public:
		qid_t registerService(Service& srvs) {
			srvs.qid = get_new_qid();
			table.put(&srvs);
			return srvs.qid;
		}

		Service* getService(qid_t qid) {
			return table.get(qid);
		}

	private: 
		qid_t get_new_qid() {
			while (table.get(++qid_counter))
				if (qid_counter > MAX_QID) qid_counter = 0; 
			return qid_counter;
		}
	};
}

#endif