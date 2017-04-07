#include <kernel/sched/SchedeeManager.h>
#include <kernel/event/TaskletManagerSchedee.h>
#include <kernel/service/MessageManagerSchedee.h>
#include <kernel/service/ServiceTable.h>
#include <kernel/sched/Scheduler.h>
#include <kernel/service/Service.h>

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