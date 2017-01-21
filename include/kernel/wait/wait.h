#ifndef GENOS_KERNEL_WAIT_H
#define GENOS_KERNEL_WAIT_H

#include <gxx/DList.h>

/*Waiter queue classes*/
class WaiterBasic {
public:
	dlist_head lnk;
	
	void remove() {
		dlist_del(&lnk);
	}
	
	virtual void unwait() = 0; 
};

class WaitQueue {
public:
	gxx::DList<WaiterBasic, &WaiterBasic::lnk> wlist;
	
	void addWaiter(WaiterBasic* waiter) {
		wlist.push_back(*waiter);
	}

	void wakeUp() {
		auto end = wlist.end();
		for (auto it = wlist.begin(); it != end; it++) {
			it->unwait();
		}
	}

	void wakeUpExclusive();

	size_t empty() {
		return wlist.empty();
	}
};

/*Basic Waiter's implementation*/
class DebugSignalWaiter : public WaiterBasic {
	void unwait() {
		debug_print("DebugSignal\r\n");
	};
};

#endif