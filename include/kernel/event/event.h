#ifndef GENOS_KERNEL_EVENT_H
#define GENOS_KERNEL_EVENT_H

#include <datastruct/dlist_head.h>
#include <compiler.h>

struct event_slot_s;
typedef void(*event_action_t)(struct event_slot_s*, void*);

typedef struct event_s {
	struct dlist_head list;
} event_t;

#define EVENT_INIT(name) { { &name.list, &name.list } }

typedef struct event_slot_s {
	//manage
	struct dlist_head lnk;

	//handler
	event_action_t action;
	void* priv;
} event_slot_t;

__BEGIN_DECLS

static inline void event_slot_init(event_slot_t* es, event_action_t action, void* arg) {
	dlist_init(&es->lnk);
	es->action = action;
	es->priv = arg;
}

static inline void event_emit(event_t * event) {
	event_slot_t *es, *next;
	dlist_for_each_entry_safe(es, next, &event->list, lnk) {
		es->action(es, es->priv);
	}
}

static inline void event_emit_one(event_t * event) {
	event_slot_t *es = dlist_first_entry(&event->list, event_slot_t, lnk);
	es->action(es, es->priv);
}

static inline void event_bind(event_slot_t* es, event_t* event) {
	dlist_add_back(&es->lnk, &event->list);
}

static inline void event_unbind(event_slot_t* es) {
	dlist_del(&es->lnk);
}

__END_DECLS

#endif