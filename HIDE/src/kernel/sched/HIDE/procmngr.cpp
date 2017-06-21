#include "kernel/sched/procmngr.h"
#include "kernel/sched/idtable.h"
#include "kernel/sched/sched.h"
#include "util/retype.h"
#include "utilxx/new.h"

idtable<virtee, &schedee::hnode, &schedee::pid, 20> process_table;

id_t process::create_autom(delegate<void> dlg, int prio) {
	RETYPE(autom_schedee*, asch, malloc(sizeof(autom_schedee)));
	new (asch) autom_schedee(dlg, prio);
	
	dlist_init_list(&asch->statelnk);
	process_table.registry(asch);

	scheduler::set_ready(asch, 0);
	asch->start();

	return asch->pid;
}

schedee* process::getsch(id_t id) {
	auto vee = process_table.find(id);
	if (vee -> type != VIRTTYPE_SCHEDEE) return 0; 
	return (schedee*) vee;	
}