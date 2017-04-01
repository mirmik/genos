#ifndef GENOS_DEBUG_SERVICE_H
#define GENOS_DEBUG_SERVICE_H

#include <kernel/service/Service.h>
#include <kernel/service/MsgApi.h>

namespace Genos {

	class DebugService : public Service {
	public:
		int8_t receiveMessage(MsgTag& msg) {
			debug_ipcstack_dump(msg.stack);
			
			replyMessage(msg);
		}

	};

}

#endif