#include <kernel/sched/SchedeeManager.h>
#include <kernel/event/TaskletManagerSchedee.h>
#include <kernel/service/MessageManagerSchedee.h>
#include <kernel/service/ServiceTable.h>
#include <kernel/sched/Scheduler.h>
#include <kernel/service/Service.h>

#include <kernel/service/MsgTag.h>

Genos::SchedeeManager 			schedeeManager;
Genos::TaskletManagerSchedee 	taskletManager;
Genos::MessageManagerSchedee 	messageManager;
Genos::ServiceTable			 	serviceTable;

Genos::AbstractSchedeeManager& Genos::Glue::systemSchedeeManager() {
	return schedeeManager;
}

void Genos::Glue::schedule() {
	schedeeManager.execute();
}

Genos::Service* Genos::Glue::getService(qid_t qid) {
	return serviceTable.getService(qid);
}

void Genos::Glue::replyMessage(MsgTag& tag) {
	messageManager.toReplyMsgTag(tag);
}

gxx::block_pool<Genos::MessageHeader, 10> msgTagPool;
gxx::block_pool<Genos::MessageStack, 10> msgStackPool;

Genos::MessageHeader* Genos::Glue::allocateMessageHeader() {
	//dprln("allocated");
	return msgTagPool.emplace();
}

void Genos::Glue::releaseMessageHeader(Genos::MessageHeader* ptr) {
	//dprln("released");	
	msgTagPool.release(ptr);
}

Genos::MessageStack* Genos::Glue::allocateMessageStack() {
	//dprln("allocated");
	return msgStackPool.emplace();
}

void Genos::Glue::releaseMessageStack(Genos::MessageStack* ptr) {
	//dprln("released");	
	msgStackPool.release(ptr);
}
