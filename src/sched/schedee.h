#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <stdint.h>
#include <stdbool.h>

#include <igris/datastruct/dlist.h>

#ifdef MVFS_INCLUDED
struct file;
#endif

#define SCHEDEE_STATE_RUN 			0
#define SCHEDEE_STATE_WAIT 			2
#define SCHEDEE_STATE_WAIT_SCHEDEE 	6
#define SCHEDEE_STATE_STOP 			3
#define SCHEDEE_STATE_FINAL 		4
#define SCHEDEE_STATE_ZOMBIE 		5

#define SCHEDEE_FDMAX 5

struct schedee_opertions;

struct schedee {
	struct schedee * parent;
	struct dlist_head lnk;
	uint8_t prio;
	uint8_t state;
	union {
		uint8_t flags;
		struct {
			uint8_t runned : 1;
			uint8_t can_displace : 1;
			uint8_t has_context : 1;
		} flag;
	};

	int local_errno;
#ifdef MVFS_INCLUDED
	struct node * pwd;
	struct file * fds [SCHEDEE_FDMAX]; //массив файловых дескрипторов
#endif

	const struct schedee_operations * sch_op;
};

struct schedee_operations {
	void (*execute) (struct schedee * sch);
	void (*displace) (struct schedee * sch);
	void (*finalize) (struct schedee * sch);
};

__BEGIN_DECLS

struct schedee * current_schedee();

/// Уведомить родителя (если он есть), что процесс завершен. 
void schedee_notify_finalize(struct schedee * sch);

/// Проинициализировать структуры данных поддержки файловой системы.
void schedee_mvfs_support_init(struct schedee* sch);

/// Переоткрыть родительские файлы от своего имени.
void schedee_copy_parent_files(struct schedee* sch);

void schedee_debug_print_fds(struct schedee* sch);

/// Проинициализировать основные поля процесса.
static inline void schedee_init(struct schedee* sch, int prio, const struct schedee_operations * op) {
	dlist_init(&sch->lnk);
	sch->prio = prio;
	sch->state = SCHEDEE_STATE_STOP;
	sch->flags = 0;
	sch->sch_op = op;

	sch->parent = current_schedee(); 

#ifdef MVFS_INCLUDED
	schedee_mvfs_support_init(sch);
	if (sch->parent)
		schedee_copy_parent_files(sch);
#endif //MVFS_INCLUDED

	sch->local_errno = 0;
} 

#ifdef MVFS_INCLUDED
int schedee_setfd(struct schedee * sch, struct file * node, int fd);
int schedee_get_free_fd(struct schedee * sch);
#endif //MVFS_INCLUDED

__END_DECLS

#endif