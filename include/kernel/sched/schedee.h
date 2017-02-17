#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <datastruct/dlist_head.h>

#define SCHEDEE_STATE_RUN 0x01
#define SCHEDEE_STATE_WAIT 0x02
#define SCHEDEE_STATE_ZOMBIE 0x04
#define SCHEDEE_STATE_MASK (SCHEDEE_STATE_RUN | SCHEDEE_STATE_WAIT | SCHEDEE_STATE_ZOMBIE) 

class schedee {
public:
	dlist_head lnk;
	uint8_t prio = 0;
	uint8_t flags = 0;

	schedee() : lnk() { }

	void Run();
	void Stop();
	void Exit();

	virtual void execute() = 0;
};

class automate_schedee : public schedee {
public:
	automate_schedee() : schedee() {  }

	virtual void execute() {
		Routine();
	}

protected:
	virtual void Routine() = 0;
};


#endif