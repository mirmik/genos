#ifndef GENOS_KERNEL_EVENT_H
#define GENOS_KERNEL_EVENT_H

#include <datastruct/dlist_head.h>
#include <compiler.h>

struct event_s;

typedef void(*event_handler_t)(void*, struct event_s*);

typedef struct event_queue_s {
	struct dlist_head list;
} event_queue_t;

#define EVENT_QUEUE_INIT(name) { { &name.list, &name.list } }

typedef struct event_s {
	//manage
	struct dlist_head lnk;

	//handler
	event_handler_t action;
	void* priv;
} event_t;

__BEGIN_DECLS

static inline void event_init(event_t* event, event_handler_t action, void* arg) {
	dlist_init(&event->lnk);
	event->action = action;
	event->priv = arg;
}

static inline void event_emit(event_queue_t * eq) {
	event_t *event, *next;
	dlist_for_each_entry_safe(event, next, &eq->list, lnk) {
		event->action(event->priv, event);
	}
}

static inline void event_bind(event_t* event, event_queue_t* eq) {
	dlist_add(&event->lnk, &eq->list);
}

static inline void event_unbind(event_t* event) {
	dlist_del(&event->lnk);
}

__END_DECLS

#endif