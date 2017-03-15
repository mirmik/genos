#include <loc_scheduler.h>

void scheduler_free(void*) __attribute__((weak));
void scheduler_free(void* ptr) { free(ptr); };

void __schedule__() {
	__label__ __schedule_loop;
	struct schedee* sch;

	__schedule_loop:

	//tasklet_schedule();

	//debug_print("Here\r\n");
	while (!dlist_empty(&finallist)) {
		struct schedee* fsch = dlist_first_entry(&finallist, struct schedee, lnk);

		//Операция final должна озаботиться извлечением процесса из списка final.
		//Иначе возможен вечный вызов данного финализатора.
		finalize_schedee(fsch);
	}

	int priolvl;
	for (priolvl = PRIORITY_TOTAL - 1; priolvl >= 0; --priolvl) {
		if (!dlist_empty(&runlist[priolvl])) goto runschedee;
	}
	//dprln(waitlist.size());
	if (dlist_empty(&waitlist)) kernel_schedule_empty();
	genos_idle();
	goto __schedule_loop;	
	//panic("nobody to run");

	runschedee:
	//debug_print("Here\r\n");
	sch = dlist_first_entry(&runlist[priolvl], struct schedee, lnk);
	
	//if (current_schedee() != &sch) current_schedee()->displace(); 	
	set_current_schedee(sch);

	//Перемещаем в конец списка.
	dlist_move_back(&sch->lnk, &runlist[priolvl]);
	
	//Если execute вернет SCHEDULE_RESUME (1), цикл завершается. 
	//SCHEDULE_REPEAT (0) ведет к следующей итерации.
	if (sch->schops->execute(sch)) {
		return;
	}
	goto __schedule_loop;
}

void __displace__() {
	__schedule__();
}