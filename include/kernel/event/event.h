#ifndef GENOS_EVENT_H
#define GENOS_EVENT_H

#include <gxx/DList.h>
#include <genos/sigslot/delegate.h>

class event_slot {
public:
	dlist_head lnk;
	virtual void routine(void* sender) = 0;
};

class event_signal {
	gxx::DList<event_slot, &event_slot::lnk> list;

public:
	void bind(event_slot& slot) {
		list.move_back(slot);
	}

	void emit(void* sender) {
		if (!list.empty()){
			auto end = list.end();
			for (auto it = list.begin(); it != end; it++) {
				it->routine(sender);
			}
		}
	}
	
	void emit_one(void* sender) {
		if (!list.empty()) list.begin()->routine(sender);
	}
};

class debug_event_slot : public event_slot {
	const char* m_message;

	void routine(void* sender) override {
		debug_print(m_message);dln();
	}

public:
	debug_event_slot(const char* message) : m_message(message){}
};

#endif