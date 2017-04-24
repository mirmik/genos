#include <kernel/sched/SchedeeManager.h>
//#include <kernel/event/TaskletManagerSchedee.h>
#include <kernel/service/MessageManager.h>
//#include <kernel/sched/SchedeeService.h>
#include <kernel/service/ServiceTable.h>
#include <kernel/sched/SchedeeTable.h>
#include <kernel/sched/Scheduler.h>
#include <kernel/service/MsgTag.h>
#include <kernel/service/MsgApi.h>

#include <gxx/pool.h>

Genos::SchedeeManager 			schedeeManager;
//Genos::TaskletManagerSchedee 	taskletManager;
//Genos::MessageManager 		 	messageManager;
Genos::ServiceTable			 	serviceTable;
Genos::SchedeeTable			 	schedeeTable;


Genos::AbstractSchedeeManager& Genos::Glue::systemSchedeeManager() {
	return schedeeManager;
}

void Genos::Glue::schedule() {
	schedeeManager.execute();
}

Genos::Service* Genos::Glue::getService(qid_t qid) {
	return serviceTable.getService(qid);
}

Genos::Schedee* Genos::Glue::getSchedee(pid_t pid) {
	return schedeeTable.get(pid);
}

Genos::SchedeeTable& Genos::Glue::systemSchedeeTable() {
	return schedeeTable;
}

//void Genos::Glue::replyMessage(Genos::MessageHeader& tag) {
	//messageManager.toReplyMsgTag(tag);
//}

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

//Genos::MessageStack* Genos::Glue::allocateMessageStack() {
	//dprln("allocated");
//	return msgStackPool.emplace();
//}

//void Genos::Glue::releaseMessageStack(Genos::MessageStack* ptr) {
	//dprln("released");	
//	msgStackPool.release(ptr);
//}

Genos::qid_t Genos::Glue::registerService(Service* srvs) {
	return serviceTable.registerService(*srvs);
}

Genos::pid_t Genos::Glue::registerSchedee(Schedee* sch) {
	return schedeeTable.registry(*sch);
}

int8_t Genos::transportQuery(MessageHeader* header) {
	dump(*header);
	
	Service* receiver = Genos::Glue::getService(header->receiver);
	if (receiver == nullptr) {
		return -1;
	}
	
	header->state.status = MessageHeader::Status::Away;
	receiver->receiveQuery(header);
	return 0;  
}

/*void Genos::transportAnswer(MessageHeader* header) {
	__label__ release;
	Service* sender;
	
	if (header->state.noanswer == true) goto release;

	sender = Genos::Glue::getService(header->sender);
	if (sender == nullptr) goto release;

	header->state.status = MessageHeader::Status::Near;
	sender->receiveAnswer(header);
	return;

	release:
	Glue::releaseMessageHeader(header);
	return;
}*/