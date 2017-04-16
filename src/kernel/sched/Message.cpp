#include <kernel/service/MessageManager.h>

int8_t Genos::Send() {
	assert(messageSlot);
	
	Genos::Glue::systemSchedeeManager().waitSchedee(*this, SCHEDEE_BLOCKED_SEND);
	
	auto ret = transportQuery(messageSlot);
	if (ret != 0) {
		Glue::systemSchedeeManager().runSchedee(*this);
		return -1;
	}

	return 0;
}

void Genos::Receive() {
	if (!queries.empty()) {
		
	}
}

void Genos::Reply() {

}