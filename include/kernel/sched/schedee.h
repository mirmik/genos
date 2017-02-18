#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <gxx/allocator.h>
#include <gxx/DList.h>
#include <genos/sigslot/delegate.h>
#include <util/stub.h>
#include <kernel/sched/schedee_ptr.h>

class schedee;
schedee* current_schedee();
void current_schedee(schedee* sch);

static constexpr uint8_t SCHEDEE_STATE_RUN = 0x01;
static constexpr uint8_t SCHEDEE_STATE_WAIT = 0x02;
static constexpr uint8_t SCHEDEE_STATE_FINAL = 0x04;
static constexpr uint8_t SCHEDEE_STATE_ZOMBIE = 0x08;
static constexpr uint8_t SCHEDEE_STATE_MASK =
	SCHEDEE_STATE_RUN | SCHEDEE_STATE_WAIT | SCHEDEE_STATE_FINAL | SCHEDEE_STATE_ZOMBIE;

static constexpr uint8_t SCHEDEE_FINAL_DEALLOCATE = 0x80;
static constexpr uint8_t SCHEDEE_FINAL_RELEASE = 0x40;

class schedee {
public:
	dlist_head lnk;
	uint8_t prio = 0;
	uint8_t flags = 0;

//	uint8_t ref = 1;

//	schedee_ptr parent;
	schedee* parent;

	void* result;

	void(*final_callback)(schedee* sch, void* result) = nullptr;

	//schedee_ptr инициализируется без инкремента счетчика ссылок,
	//т.к. инициализируется raw указателем.
	schedee( schedee* parent );

 	virtual ~schedee() { }

	schedee& Result(void* res);
	void* Result();
	
	schedee& Run();
	schedee& Stop();
	void Exit();
	//void release();

	schedee& final_deallocate(bool en);
	schedee& final_release(bool en);
	
	schedee& final_handler(void(*)(schedee*, void*));
	schedee& final_handler(void(*)());

	bool Completed();
	bool operator==(const schedee& other) { return this == &other; }

	virtual void execute() = 0;
	virtual void invalidate() = 0;
	void final();
};

class automate_schedee : public schedee {
protected:
	automate_schedee( schedee* parent = current_schedee() ) : schedee(parent) {  }
	~automate_schedee() { }

protected:
	void execute() override {
		Routine();
	}

	virtual void invalidate() override {}
	virtual void Routine() = 0;

};

class delegate_schedee : public schedee {
public:
	delegate_schedee(delegate<void> dlg, schedee* parent = current_schedee() ) : schedee( parent ), m_dlg(dlg) {  }
	~delegate_schedee() { }

private:
	virtual void execute() override {
		m_dlg();
	}

	delegate<void> m_dlg;

protected:
	virtual void invalidate() override {}
};


static void resume_parent_schedee(schedee* sch, void* result) {
	sch -> parent -> Result(result);
	sch -> parent -> Run();
}

extern gxx::DList<schedee, &schedee::lnk> waitlist;
static void resume_parent_schedee_safe(schedee* sch, void* result) {
	if ( gxx::find(waitlist.begin(), waitlist.end(), *sch) != waitlist.end()) 
		resume_parent_schedee(sch, result);
}

#endif