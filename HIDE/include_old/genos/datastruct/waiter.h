#ifndef GENOS_WAITER_H
#define GENOS_WAITER_H

#define WAITER_STOP 0
#define WAITER_WAIT 1

#include <gxx/util/stub.h>

class waiter {
	volatile uint8_t flag;
	void (*func)(uintptr_t arg);
	uintptr_t arg;
public:

	inline void wait() {
		flag = WAITER_WAIT;
	}

	inline void exec() {
		stop();
		func(arg);
	}

	inline void stop() {
		flag = WAITER_STOP;
	}

	inline waiter() : func(do_nothing), arg(0) {};
	inline waiter(void (*_func)(uintptr_t), uintptr_t _arg) : func(_func), arg(_arg) {};
};

class procwaiter : public waiter {
	inline procwaiter() : waiter() {} 
};

#endif