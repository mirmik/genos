#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <genos/ctrobj.h>
#include <genos/wait.h>
#include <genos/ktimer.h>

#include <genos/resource.h>
#include <genos/nav.h>

#define SCHEDEE_STATE_RUN 			0
#define SCHEDEE_STATE_WAIT 			2
#define SCHEDEE_STATE_WAIT_SCHEDEE 	6
#define SCHEDEE_STATE_STOP 			3
#define SCHEDEE_STATE_FINAL 		4
#define SCHEDEE_STATE_ZOMBIE 		5

#define SCHEDEE_FDMAX 5

extern struct dlist_head schedee_list;

namespace genos
{
	struct schedee
	{
		genos::schedee * parent;

		union
		{
			struct ctrobj ctr;
			struct waiter waiter;
			struct ktimer_base ktimer;
		};

		uint8_t prio;
		uint8_t sch_state;

#if SCHEDEE_DEBUG_STRUCT
		struct dlist_head schedee_list_lnk;
		uint16_t dispcounter;
		uint16_t execcounter;
#endif

		union
		{
			uint8_t flags;
			struct
			{
				uint8_t runned 			: 1;
				uint8_t can_displace 	: 1;
				uint8_t has_context 	: 1;
				uint8_t dynamic 		: 1;
				uint8_t dynamic_heap 	: 1;
				uint8_t killed			: 1;
			} flag;
		};

		int16_t local_errno;

		genos::restbl restbl;
		genos::navblock * navblock = nullptr;

		schedee() {}

		virtual void execute() = 0;
		virtual int displace() = 0;
		virtual void finalize() = 0;
		virtual void signal_handler(int sig) {}

		//void set_restbl(genos::openres* tbl, uint8_t tblsize)
		//{ restbl.set_table(tbl, tblsize); }

		void set_navblock(genos::navblock * nav) { navblock = nav; }

		genos::openres * getres(int i) 
		{
			return restbl[i];
		}

		void run();
	};
}

__BEGIN_DECLS

genos::schedee * current_schedee();

/// Уведомить родителя (если он есть), что процесс завершен.
void schedee_notify_finalize(genos::schedee * sch);

/// Переоткрыть родительские файлы от своего имени.
void schedee_copy_parent_files(genos::schedee* sch);

void schedee_debug_print_fds(genos::schedee* sch);

/// Проинициализировать основные поля процесса.
static inline
void schedee_init(genos::schedee* sch, int prio)
{

#if SCHEDEE_DEBUG_STRUCT

	// В дальнейшем эту провеку следует убрать, так как нод
	// должен отстыковываться от списка по завершению работы.
	if (!dlist_in(&sch->schedee_list_lnk, &schedee_list))
	{
		ctrobj_init(&sch->ctr, CTROBJ_SCHEDEE_LIST);
		dlist_add(&sch->schedee_list_lnk, &schedee_list);
	}

	else
	{
		dlist_del_init(&sch->ctr.lnk);
	}

	sch->dispcounter = 0;
	sch->execcounter = 0;
#else
	ctrobj_init(&sch->ctr, CTROBJ_SCHEDEE_LIST);
#endif

	sch->prio = prio;
	sch->sch_state = SCHEDEE_STATE_STOP;
	sch->flags = 0;

	sch->parent = current_schedee();

	sch->local_errno = 0;
}

#if SCHEDEE_DEBUG_STRUCT
void schedee_list_debug_info();
#endif


__END_DECLS

#endif