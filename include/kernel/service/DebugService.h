#ifndef GENOS_DEBUG_SERVICE_H
#define GENOS_DEBUG_SERVICE_H

#include <kernel/service/Service.h>
//#include <kernel/service/MsgApi.h>

namespace Genos {

	class DebugService : public Service {
	public:
		int8_t receiveMessage(MsgTag& msg) {
			dprln("DebugService::receiveMessage");
			debug_ipcstack_dump(msg.stack);
			ReplyMessage(msg);
		}
	};
}

#endif