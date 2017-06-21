#ifndef GENOS_SERVICE_H
#define GENOS_SERVICE_H

#include <kernel/service/MsgTag.h>
#include <kernel/service/MessageManager.h>
#include <datastruct/hlist_head.h>
#include <kernel/id/id.h>
#include <kernel/csection.h>
#include <kernel/panic.h>

#define WAIT_REPLY 0
#define FAST_REPLY 1
#define ERROR_REPLY -1

namespace Genos {
	
	class Service {
	public:
		struct hlist_node hlnk;
		qid_t qid;
		uint8_t prio;
		
	public:
		virtual int8_t receiveQuery(MessageHeader *tag) = 0;
		
		//hashtable support
		static qid_t& getkey(Service& srvs) { return srvs.qid; }
	};	

	namespace Glue {
		qid_t registerService(Service* srvs);
	}
}

#endif