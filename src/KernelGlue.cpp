#include <sched/SchedeeManager.h>
//#include <kernel/event/TaskletManagerSchedee.h>
//#include <kernel/service/MessageManager.h>
//#include <kernel/sched/SchedeeService.h>
//#include <kernel/service/ServiceTable.h>
#include <sched/SchedeeTable.h>
#include <sched/Scheduler.h>
//#include <kernel/service/MsgTag.h>
//#include <kernel/service/MsgApi.h>

//#include <gxx/pool.h>

Genos::SchedeeManager 			schedeeManager;
//Genos::TaskletManagerSchedee 	taskletManager;
//Genos::MessageManager 		 	messageManager;
//Genos::ServiceTable			 	serviceTable;
Genos::SchedeeTable			 	schedeeTable;


Genos::AbstractSchedeeManager& Genos::Glue::systemSchedeeManager() {
	return schedeeManager;
}

void genos::sched::schedule() {
	schedeeManager.execute();
}

//Genos::Service* Genos::Glue::getService(qid_t qid) {
//	return serviceTable.getService(qid);
//}

Genos::Schedee* Genos::Glue::getSchedee(pid_t pid) {
	return schedeeTable.get(pid);
}

Genos::SchedeeTable& Genos::Glue::systemSchedeeTable() {
	return schedeeTable;
}

//void Genos::Glue::replyMessage(Genos::MessageHeader& tag) {
	//messageManager.toReplyMsgTag(tag);
//}

/*gxx::block_pool<Genos::MessageHeader, 10> msgTagPool;
gxx::block_pool<Genos::MessageStack, 10> msgStackPool;

Genos::MessageHeader* Genos::Glue::allocateMessageHeader() {
	//dprln("allocated");
	return msgTagPool.emplace();
}*/

//void Genos::Glue::releaseMessageHeader(Genos::MessageHeader* ptr) {
	//dprln("released");	
//	msgTagPool.release(ptr);
//}

//Genos::MessageStack* Genos::Glue::allocateMessageStack() {
	//dprln("allocated");
//	return msgStackPool.emplace();
//}

//void Genos::Glue::releaseMessageStack(Genos::MessageStack* ptr) {
	//dprln("released");	
//	msgStackPool.release(ptr);
//}

/*Genos::qid_t Genos::Glue::registerService(Service* srvs) {
	return serviceTable.registerService(*srvs);
}*/

genos::pid_t Genos::Glue::registerSchedee(Schedee* sch) {
	return schedeeTable.registry(*sch);
}

/*int8_t Genos::transportQuery(MessageHeader* header) {
	dump(*header);
	
	Service* receiver = Genos::Glue::getService(header->receiver);
	if (receiver == nullptr) {
		return -1;
	}
	
	header->state.status = MessageHeader::Status::Away;
	receiver->receiveQuery(header);
	return 0;  
}*/

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


//TODO: replace section
/*#include <kernel/event/WakeUpWaiter.h>
#include <kernel/event/Waiter.h>
#include <kernel/csection.h>*/

/*void Genos::wakeup_wait(WaiterHead& head, WakeUpWaiter& wakeup) {
	atomic_section_enter();
	wakeup.schedee()->addResource(wakeup);
	wakeup.schedee()->wait();
	head.wait(wakeup);
	atomic_section_leave();
}

int8_t Genos::wakeup_wait(pid_t pid, WakeUpWaiter& wakeup) {
	atomic_section_enter();
	auto sch = Genos::raw(pid);
	if (!sch) return -1;
	wakeup_wait(sch->finalWaiterHead, wakeup);
	atomic_section_leave();
}*/