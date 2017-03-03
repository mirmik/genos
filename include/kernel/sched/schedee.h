#ifndef GENOS_SCHEDEE_H
#define GENOS_SCHEDEE_H

#include <gxx/allocator.h>
#include <gxx/DList.h>
#include <genos/sigslot/delegate.h>
#include <util/stub.h>
#include <kernel/sched/schedee_ptr.h>

#include <kernel/service/service.h>

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

class schedee : public service {
public:
	dlist_head lnk;
	uint8_t prio = 0;
	uint8_t flags = 0;

//	uint8_t ref = 1;

//	schedee_ptr parent;
	//void* argument;
	//void* result;

	void(*final_callback)(schedee* sch) = nullptr;

	//schedee_ptr инициализируется без инкремента счетчика ссылок,
	//т.к. инициализируется raw указателем.
	schedee();

 	virtual ~schedee() { }

	schedee& Result(void* res);
	void* Result();
	
	schedee& Run();
	schedee& Stop();
	void Exit();
	//void release();

	schedee& final_deallocate(bool en);
	schedee& final_release(bool en);
	
	schedee& final_handler(void(*)(schedee*));

	template<typename F> 
	schedee& final_handler(F f) {
		return final_handler((void(*)(schedee*)) f);
	}

	schedee& final_argument(void*);

	bool Completed();
	bool operator==(const schedee& other) { return this == &other; }

	virtual void execute() = 0;
	virtual void invalidate() = 0;
	void final();
};



class delegate_schedee : public schedee {
public:
	delegate_schedee(delegate<void> dlg) : m_dlg(dlg) {  }
	~delegate_schedee() { }

private:
	virtual void execute() override {
		m_dlg();
	}

	delegate<void> m_dlg;

protected:
	virtual void invalidate() override {}
};


//static void resume_parent_schedee(schedee* sch) {
//	((schedee*)sch->argument) -> Run();
//}

//extern gxx::DList<schedee, &schedee::lnk> waitlist;
//static void resume_parent_schedee_safe(schedee* sch) {
//	if ( gxx::find(waitlist.begin(), waitlist.end(), *sch) != waitlist.end()) 
//		resume_parent_schedee(sch);
//}

#include <kernel/panic.h>
class stub_schedee : public schedee {
public:
	void execute() {
		panic("stubschedee_execute");
	}

	void invalidate() {
		panic("stubschedee_invalidate");
	}

	stub_schedee();
};

static int stub_schedee_send_query(struct service* ths, struct query *q) {
	struct stub_schedee *sch = (stub_schedee*) ths;
	int ret = kernel_transport_query(q);
	switch (ret) {
		case WAIT_REPLY : panic("stub schedee can`t waited");
		case FAST_REPLY : return 0;
		case ERROR_REPLY : return q->errstat;
	}
}

static const struct client_operations stub_schedee_service_operations = {
	.send_query = stub_schedee_send_query,
	do_nothing
};

#endif