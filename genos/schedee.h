#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <igris/osinter/ctrobj.h>
#include <igris/osinter/wait.h>
#include <genos/ktimer.h>

#ifndef SCHEDEE_DEBUG_STRUCT
#define SCHEDEE_DEBUG_STRUCT 1
#endif

#define SCHEDEE_PRIORITY_TOTAL 4

#define SCHEDEE_STATE_RUN 			0
#define SCHEDEE_STATE_WAIT 			2
#define SCHEDEE_STATE_WAIT_SCHEDEE 	6
#define SCHEDEE_STATE_STOP 			3
#define SCHEDEE_STATE_FINAL 		4
#define SCHEDEE_STATE_ZOMBIE 		5

#define SCHEDEE_FDMAX 5

#define SCHEDEE_USE_PARENT_GID 1

extern struct dlist_head schedee_list;


struct resource_table;
struct navigation_block;
struct schedee;
struct openres;

namespace genos
{
	class schedee
	{
	public:
		struct schedee * parent;
		void (*signal_handler) (int sig);

		union
		{
			struct ctrobj      ctr;
			struct waiter      waiter;
			struct ktimer_head ktimer;
		};

		uint8_t prio;
		uint8_t sch_state;

		uint16_t pid;
		uint16_t gid;

#if SCHEDEE_DEBUG_STRUCT
		const char * mnemo;
		struct dlist_head schedee_list_lnk;
		uint16_t dispcounter;
		uint16_t execcounter;
#endif

		int local_errno;

		// Ресурсы должны принадлежать процессу специального вида (процесс-пользователь).
		// всем schedee ресурсы не нужны.
		struct resource_table * restbl;
		struct navigation_block * navblock;

	public:
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
		uint8_t syslock_counter_save;

	public:
		schedee(int prio, int flags);

		virtual void execute() = 0;
		virtual void finalize() = 0;
		virtual int displace() = 0;
	};

	struct schedee * current_schedee();

	void schedee_manager_init();
	void schedee_manager_step();

	void schedee_start(struct schedee * sch);
	void schedee_pause(struct schedee * sch);
	void schedee_deinit(struct schedee * sch);

#if SCHEDEE_DEBUG_STRUCT
	void schedee_list_debug_info();
#endif

	int schedee_get_free_openres(struct schedee * sch, struct openres ** res);
}

__BEGIN_DECLS
void __schedule__();
__END_DECLS

#endif