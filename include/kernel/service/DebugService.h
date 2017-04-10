#ifndef GENOS_DEBUG_SERVICE_H
#define GENOS_DEBUG_SERVICE_H

#include <kernel/service/Service.h>
#include <kernel/devices/serial/Stream.h>
//#include <kernel/service/MsgApi.h>

namespace Genos {

	class DebugService : public Service {
	public:
		int8_t receiveMessageHandler(MsgTag&& tag) override {
			//MsgTag tag = &header;
			
			dprln("DebugService::receiveMessage");
			MsgBody stack = tag.get();

			stack->dump(Genos::Glue::debugStream());

			//debug_ipcstack_dump(msg.stack);
			//ReplyMessage(msg);
		}
	};
}

#endif