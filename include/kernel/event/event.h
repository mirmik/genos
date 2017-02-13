#ifndef GENOS_EVENT_H
#define GENOS_EVENT_H

#include <gxx/DList.h>
#include <genos/sigslot/delegate.h>

class event_slot {
public:
	dlist_head lnk;
	fastdelegate<void> m_dlg;

	event_slot(fastdelegate<void> dlg) : m_dlg(dlg) {}
	event_slot(void(*func)(void*), void* arg) : m_dlg(func, arg) {}
};

class event_signal {
	gxx::DList<event_slot, &event_slot::lnk> list;

public:
	void bind(event_slot& slot) {
		list.push_back(slot);
	}

	void emit() {}
	
	void emit_one() {
		if (!list.empty()) list.begin()->m_dlg();
	}
};

#endif