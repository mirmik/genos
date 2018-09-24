#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#define SCHEDEE_STATE_RUN 		0
#define SCHEDEE_STATE_WAIT 		2
#define SCHEDEE_STATE_STOP 		3
#define SCHEDEE_STATE_FINAL 	4
#define SCHEDEE_STATE_ZOMBIE 	5

struct schedee_opertions;

struct schedee {
	struct dlist_head lnk;
	uint8_t prio;
	uint8_t state;
	bool runned;

	const struct schedee_operations * sch_op;
};

struct schedee_operations {
	void (*execute) (struct schedee * sch);
	void (*displace) (struct schedee * sch);
	void (*finalize) (struct schedee * sch);
};

__BEGIN_DECLS

static inline void schedee_init(struct schedee* sch, int prio, const struct schedee_operations * op) {
	dlist_init(&sch->lnk);
	sch->prio = prio;
	sch->state = SCHEDEE_STATE_STOP;
	sch->runned = false;
	sch->sch_op = op;
}

__END_DECLS

#endif