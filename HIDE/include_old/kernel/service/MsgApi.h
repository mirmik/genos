#ifndef GENOS_MESSAGE_API_H
#define GENOS_MESSAGE_API_H

#include <kernel/service/MsgTag.h>
#include <kernel/service/Service.h>

namespace Genos {

	void currentService(Service* service);
	Service* currentService();

	//void noReplySend(MsgTag& tag);

	namespace Glue {
		//void messageHeaderToSend(MessageHeader& header);
	}

}

#endif