#include <kernel/service/MsgApi.h>

Genos::Service* __current_service = nullptr;

void Genos::currentService(Service* service) {
	__current_service = service;
}

Genos::Service* Genos::currentService() {
	return __current_service;
}

void Genos::noReplySend(MsgTag& tag) {
	tag->state.noanswer = true;
	tag->sender = __current_service->qid;
	Genos::Glue::messageHeaderToSend(*tag.header.get()); 
}